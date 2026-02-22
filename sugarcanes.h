#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define humid 2
#define water 1
#define dry 0

struct Matrix {
	int rows;
	int cols;
	int* values;
};

extern void sugarcanes(const int length, const int width);