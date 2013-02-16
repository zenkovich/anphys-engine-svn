#ifndef SEQUENTIAL_IMPULSES_SOLVER_H
#define SEQUENTIAL_IMPULSES_SOLVER_H

#include "solver_interface.h"

struct phSequentialImpulsesSolver:public phConstraintsSolverInterface
{
	phSequentialImpulsesSolver():phConstraintsSolverInterface() {}
	phSequentialImpulsesSolver(phScene* scene):phConstraintsSolverInterface(scene) {}

	void solveConstraints(float performance, float dt);
};

#endif //SEQUENTIAL_IMPULSES_SOLVER_H