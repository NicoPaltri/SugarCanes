#include "sugarcanes.h"

static inline bool in_bounds(const struct Matrix* m, int r, int c) {
    return r >= 0 && r < m->rows && c >= 0 && c < m->cols;
}

static inline int idx_of(const struct Matrix* m, int r, int c) {
    return r * m->cols + c;
}

static void printMatrix(const struct Matrix* matrix, int humidBlocks, int waterSource) {
    for (int r = 0; r < matrix->rows; r++) {
        for (int c = 0; c < matrix->cols; c++) {
            printf("%d ", matrix->values[r * matrix->cols + c]);
        }
        printf("\n");
    }

    double dimension = (double)matrix->cols * (double)matrix->rows;
    double percentage = (dimension == 0.0) ? 0.0 : ((double)humidBlocks / dimension * 100.0);

    printf("La soluzione ottima prevede %d blocchi umidi su %.0f blocchi totali (%.2f%%)\n",
        humidBlocks, dimension, percentage);
    printf("In particolare: %d blocchi umidi, %d fonti d'acqua, %.0f blocchi aridi\n",
        humidBlocks, waterSource, dimension - humidBlocks - waterSource);
}

static bool cell_should_be_humid(const struct Matrix* m, int r, int c) {
    if (!in_bounds(m, r, c)) return false;

    const int v = m->values[idx_of(m, r, c)];
    if (v == water) return false;

    if (in_bounds(m, r - 1, c) && m->values[idx_of(m, r - 1, c)] == water) return true;
    if (in_bounds(m, r + 1, c) && m->values[idx_of(m, r + 1, c)] == water) return true;
    if (in_bounds(m, r, c - 1) && m->values[idx_of(m, r, c - 1)] == water) return true;
    if (in_bounds(m, r, c + 1) && m->values[idx_of(m, r, c + 1)] == water) return true;

    return false;
}

static int recompute_cell(struct Matrix* m, int r, int c) {
    if (!in_bounds(m, r, c)) return 0;

    const int i = idx_of(m, r, c);
    if (m->values[i] == water) return 0;

    const int before = m->values[i];
    const int after = cell_should_be_humid(m, r, c) ? humid : dry;

    if (before == after) return 0;

    m->values[i] = after;
    if (before == humid && after == dry) return -1;
    if (before == dry && after == humid) return +1;
    return 0;
}

static void solve(
    struct Matrix* m,
    int pos,
    int humidCount,
    int waterCount,
    int lockedDryCount,
    int* bestValues,
    int* bestHumid,
    int* bestWater
) {
    const int dim = m->rows * m->cols;

    // Calcolo dei blocchi aridi liberi (potenzialmente convertibili)
    int remainingDry = dim - waterCount - lockedDryCount - humidCount;

    //almeno 1/5 dei blocchi aridi rimanenti dovr� essere sacrificato come acqua
    int max_possible_humid = dim - waterCount - lockedDryCount - (remainingDry / 5);

    if (max_possible_humid < *bestHumid) return;
    if (max_possible_humid == *bestHumid && waterCount >= *bestWater) return;

    if (pos == dim) {
        if (humidCount > *bestHumid || (humidCount == *bestHumid && waterCount < *bestWater)) {
            *bestHumid = humidCount;
            *bestWater = waterCount;
            memcpy(bestValues, m->values, (size_t)dim * sizeof(int));
        }
        return;
    }

    const int r = pos / m->cols;
    const int c = pos % m->cols;

    // A) NESSUNA SORGENTE D'ACQUA
    int newly_locked_dry_A = 0;
    if (r > 0 && m->values[idx_of(m, r - 1, c)] == dry) newly_locked_dry_A++;
    if (r == m->rows - 1 && c > 0 && m->values[idx_of(m, r, c - 1)] == dry) newly_locked_dry_A++;
    if (r == m->rows - 1 && c == m->cols - 1 && m->values[pos] == dry) newly_locked_dry_A++;

    solve(m, pos + 1, humidCount, waterCount, lockedDryCount + newly_locked_dry_A, bestValues, bestHumid, bestWater);

    // B) PIAZZIAMO LA SORGENTE D'ACQUA
    const int ar[5] = { r, r - 1, r + 1, r, r };
    const int ac[5] = { c, c, c, c - 1, c + 1 };
    int old[5];
    for (int k = 0; k < 5; k++) {
        if (in_bounds(m, ar[k], ac[k])) old[k] = m->values[idx_of(m, ar[k], ac[k])];
        else old[k] = -1;
    }

    const int cell_i = idx_of(m, r, c);
    if (m->values[cell_i] != water) {
        int newHumid = humidCount;
        int newWater = waterCount;

        if (m->values[cell_i] == humid) newHumid -= 1;

        m->values[cell_i] = water;
        newWater += 1;

        newHumid += recompute_cell(m, r - 1, c);
        newHumid += recompute_cell(m, r + 1, c);
        newHumid += recompute_cell(m, r, c - 1);
        newHumid += recompute_cell(m, r, c + 1);

        int newly_locked_dry_B = 0;
        // Ricalcoliamo i blocchi permanentemente aridi in base al nuovo stato della griglia
        if (r > 0 && m->values[idx_of(m, r - 1, c)] == dry) newly_locked_dry_B++;
        if (r == m->rows - 1 && c > 0 && m->values[idx_of(m, r, c - 1)] == dry) newly_locked_dry_B++;

        solve(m, pos + 1, newHumid, newWater, lockedDryCount + newly_locked_dry_B, bestValues, bestHumid, bestWater);
    }

    // UNDO
    for (int k = 0; k < 5; k++) {
        if (old[k] != -1) m->values[idx_of(m, ar[k], ac[k])] = old[k];
    }
}

void sugarcanes(const int length, const int width) {
    if (length <= 0 || width <= 0) return;

    struct Matrix* matrix = NULL;
    int* bestSolution = NULL;

    const int dimension = length * width;

    matrix = malloc(sizeof(struct Matrix));
    if (!matrix) goto cleanup;

    matrix->rows = length;
    matrix->cols = width;
    matrix->values = calloc((size_t)dimension, sizeof(int));
    if (!matrix->values) goto cleanup;

    bestSolution = calloc((size_t)dimension, sizeof(int));
    if (!bestSolution) goto cleanup;

    int bestHumid = 0;
    int bestWater = dimension;

    solve(matrix, 0, 0, 0, 0, bestSolution, &bestHumid, &bestWater);

    memcpy(matrix->values, bestSolution, (size_t)dimension * sizeof(int));

    printMatrix(matrix, bestHumid, bestWater);

cleanup:
    free(bestSolution);
    if (matrix) {
        free(matrix->values);
        free(matrix);
    }
}