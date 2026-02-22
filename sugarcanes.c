#include"sugarcanes.h"

void printMatrix(struct Matrix* matrix, int humidBlocks, int waterSource) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			printf("%d ", matrix->values[r * matrix->cols + c]);
		}
		printf("\n");
	}

	double dimension = matrix->cols * matrix->rows;
	double dValueHumidBlocks = humidBlocks;
	double percentage = dValueHumidBlocks / dimension * 100;

	printf("La soluzione ottima prevede %d blocchi umidi su %.0f blocchi totali (%.2f%%)\n", humidBlocks, dimension, percentage);
	printf("Specifically: %d blocchi umidi, %d fonti d'acqua, %.0f blocchi aridi", humidBlocks, waterSource, dimension - humidBlocks - waterSource);
}

int humidify(struct Matrix* matrix, int rSource, int cSource) {
	int humidifiedBlock = 0;

	int *blockAbove = &matrix->values[(rSource - 1) * matrix->cols + cSource];
	int *blockBelow = &matrix->values[(rSource + 1) * matrix->cols + cSource];
	int *blockLeft = &matrix->values[rSource * matrix->cols + cSource - 1];
	int *blockRight = &matrix->values[rSource * matrix->cols + cSource + 1];

	if (rSource > 0) {
		if (*blockAbove == dry) {
			*blockAbove = humid;
			humidifiedBlock++;
		}
	}

	if (rSource < matrix->rows - 1) {
		if (*blockBelow == dry) {
			*blockBelow = humid;
			humidifiedBlock++;
		}
	}

	if (cSource > 0) {
		if (*blockLeft == dry) {
			*blockLeft = humid;
			humidifiedBlock++;
		 }
	}

	if (cSource < matrix->cols - 1) {
		if (*blockRight == dry) {
			*blockRight = humid;
			humidifiedBlock++;
		}
	}

	return humidifiedBlock;
}

bool shouldBeHumid(struct Matrix* matrix, int rSource, int cSource) {

	if (rSource < 0 || rSource >= matrix->rows || cSource < 0 || cSource >= matrix->cols) {
		return true;
	}

	int blockAbove = matrix->values[(rSource - 1) * matrix->cols + cSource];
	int blockBelow = matrix->values[(rSource + 1) * matrix->cols + cSource];
	int blockLeft = matrix->values[rSource * matrix->cols + cSource - 1];
	int blockRight = matrix->values[rSource * matrix->cols + cSource + 1];

	if (rSource > 0) {
		if (blockAbove == water) {
			return true;
		}
	}

	if (rSource < matrix->rows - 1) {
		if (blockBelow == water) {
			return true;
		}
	}

	if (cSource > 0) {
		if (blockLeft == water) {
			return true;
		}
	}

	if (cSource < matrix->cols - 1) {
		if (blockRight == water) {
			return true;
		}
	}

	return false;
}

int drain(struct Matrix* matrix, int rSource, int cSource) {
	int drainedBlocks = 0;

	int *blockAbove = &matrix->values[(rSource - 1) * matrix->cols + cSource];
	int *blockBelow = &matrix->values[(rSource + 1) * matrix->cols + cSource];
	int *blockLeft = &matrix->values[rSource * matrix->cols + cSource - 1];
	int *blockRight = &matrix->values[rSource * matrix->cols + cSource + 1];

	if (!shouldBeHumid(matrix, rSource - 1, cSource)) {
		*blockAbove = dry;
		drainedBlocks++;
	}

	if (!shouldBeHumid(matrix, rSource + 1, cSource)) {
		*blockBelow = dry;
		drainedBlocks++;
	}

	if (!shouldBeHumid(matrix, rSource, cSource-1)) {
		*blockLeft = dry;
		drainedBlocks++;
	}

	if (!shouldBeHumid(matrix, rSource, cSource+1)) {
		*blockRight = dry;
		drainedBlocks++;
	}

	return drainedBlocks;
}

void recursiveSugarCanes(struct Matrix* matrix, int humidBlocks, int waterSource, int* bestSolution, int* maxNumberHumidBlock, int* bestWaterSource, int row, int col) {
	if (col >= matrix->cols) {
		col -= matrix->cols;
		row++;
	}
	
	if (humidBlocks >= *maxNumberHumidBlock) {
		if ((humidBlocks == *maxNumberHumidBlock && waterSource < *bestWaterSource) || humidBlocks > *maxNumberHumidBlock) {
			*maxNumberHumidBlock = humidBlocks;
			*bestWaterSource = waterSource;
			memcpy(bestSolution, matrix->values, matrix->rows * matrix->cols * sizeof(int));
		}

	}

	if (row >= matrix->rows) {
		return;
	}

	if (humidBlocks < *maxNumberHumidBlock && waterSource >= *bestWaterSource) {
		return;
	}

	for (int r = row; r < matrix->rows; r++) {
		for (int c = col; c < matrix->cols; c++) {
			if (matrix->values[r * matrix->cols + c] == water || matrix->values[r * matrix->cols + c] == humid) {
				continue;
			}

			//SET
			matrix->values[r * matrix->cols + c] = water;
			humidBlocks +=  humidify(matrix, r, c);
			recursiveSugarCanes(matrix, humidBlocks, waterSource + 1, bestSolution, maxNumberHumidBlock, bestWaterSource, r, c + 2);

			//RESET
			matrix->values[r * matrix->cols + c] = dry;
			humidBlocks -= drain(matrix, r, c);
			recursiveSugarCanes(matrix, humidBlocks, waterSource, bestSolution, maxNumberHumidBlock, bestWaterSource, r, c + 1);
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

	int maxNumberHumidBlock = 0;
	int bestWaterSource = dimension;

	recursiveSugarCanes(matrix, 0, 0, bestSolution, &maxNumberHumidBlock, &bestWaterSource, 0, 0);

	memcpy(matrix->values, bestSolution, dimension * sizeof(int));
	printMatrix(matrix, maxNumberHumidBlock, bestWaterSource);

	free(bestSolution);
	free(matrix->values);
	free(matrix);

	return;
}