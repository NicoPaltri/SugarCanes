#include"sugarcanes.h"

void printMatrix(struct Matrix* matrix) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			printf("%d ", matrix->values[r * c + c]);
		}
		printf("\n");
	}
}

bool verifySolution(struct Matrix* matrix) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			if (matrix->values[r * c + c] == dry) {
				return false;
			}
		}
	}

	return true;
}

void humidify(struct Matrix* matrix, int rSource, int cSource) {
	if (rSource != 0) {
		matrix->values[(rSource - 1) * matrix->cols + cSource] = matrix->values[(rSource - 1) * matrix->cols + cSource] == water ? water : humid;
	}

	if (rSource != matrix->rows - 1) {
		matrix->values[(rSource + 1) * matrix->cols + cSource] = matrix->values[(rSource + 1) * matrix->cols + cSource] == water ? water : humid;
	}

	if (cSource != 0) {
		matrix->values[rSource * matrix->cols + cSource - 1] = matrix->values[rSource * matrix->cols + cSource - 1] == water ? water : humid;
	}

	if (cSource != matrix->cols - 1) {
		matrix->values[rSource * matrix->cols + cSource + 1] = matrix->values[rSource * matrix->cols + cSource + 1] == water ? water : humid;
	}
}

bool isWater(struct Matrix* matrix, int rSource, int cSource) {
	if (rSource < 0) {
		return false;
	}

	if (cSource >= matrix->cols) {
		cSource -= matrix->cols;
		rSource += 1;
	}

	if (rSource >= matrix->rows) {
		return false;
	}

	return matrix->values[rSource * matrix->cols + cSource] == water;
}

void drain(struct Matrix* matrix, int rSource, int cSource) {

	if (isWater(matrix,rSource-1,cSource)) {
		return;
	}

	if (isWater(matrix, rSource+1, cSource)) {
		return;
	}

	if (isWater(matrix, rSource, cSource-1)) {
		return;
	}

	if (isWater(matrix, rSource, cSource+1)) {
		return;
	}

	matrix->values[rSource *matrix->cols+cSource]=dry;
}

void recursiveSugarCanes(struct Matrix* matrix, int waterSource, int* bestSolution, int* bestWaterSource, int row, int col) {
	if (waterSource > matrix->rows * matrix->cols / 2) {
		return;
	}

	if (row * col + col > matrix->rows * matrix->cols + matrix->cols) {
		bool isSolutionViable = verifySolution(matrix);

		if (!isSolutionViable) {
			return;
		}

		if (waterSource >= *bestWaterSource) {
			return;
		}

		*bestWaterSource = waterSource;
		memcpy(bestSolution, matrix->values, matrix->rows * matrix->cols * sizeof(int));

		return;
	}

	if (col >= matrix->cols) {
		col -= matrix->cols;
		row++;
	}

	for (int r = row; r < matrix->rows; r++) {
		for (int c = col; c < matrix->cols; c++) {
			if (matrix->values[r * matrix->cols + c] == water || matrix->values[r * matrix->cols + c] == humid) {
				continue;
			}

			matrix->values[r * matrix->cols + c] = water;
			humidify(matrix, r, c);
			recursiveSugarCanes(matrix, waterSource + 1, bestSolution, bestWaterSource, r, c + 2);

			matrix->values[r * matrix->cols + c] = humid;
			drain(matrix, r, c);
			recursiveSugarCanes(matrix, waterSource, bestSolution, bestWaterSource, r, c + 1);
		}
	}
}


void sugarcanes(const int lenght, const int width) {
	if (lenght <= 0 || width <= 0) {
		return;
	}

	int dimension = lenght * width;	
	struct Matrix* matrix = malloc(dimension * sizeof(struct Matrix));
	if (!matrix) {
		return;
	}

	matrix->rows = lenght;
	matrix->cols = width;
	matrix->values = calloc(dimension, sizeof(int));
	if (!matrix->values) {
		free(matrix);
		return;
	}

	int* bestSolution = calloc(dimension, sizeof(int));
	if (!bestSolution) {
		free(matrix->values);
		free(matrix);
		return;
	}

	int bestWaterSource = dimension;

	recursiveSugarCanes(matrix,0, bestSolution, &bestWaterSource, 0, 0);

	memcpy(matrix->values, bestSolution, dimension * sizeof(int));
	printMatrix(matrix);

	free(bestSolution);
	free(matrix->values);
	free(matrix);

	return;
}