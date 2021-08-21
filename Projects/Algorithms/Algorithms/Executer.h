#pragma once

#include "Algorithms.h"

class Executer
{
public:

	static void buildConvexHullForGeneratedData();

	static void testMAPGreedy();

	static void runMapGreedy();

	static void testTriangulation();

	static void runTriangulation();

	static void testDAC_Perm();

	static void runMAPDAC();

	static void runMAPDAC2();

	static void runMAP_RAND();

	static void testMAP_RS();

	static void runMAP_RS();

	static void runMAP_Postprocess();

	static void compareResults();

	static void showPostprocessingPersentage();

	static void forFigures1();
	static void forFigures2();

	static void forFigure1_real();
	static void forFigure2_real();
};