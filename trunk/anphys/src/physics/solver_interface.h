#ifndef SOLVER_INTERFACE_H
#define SOLVER_INTERFACE_H

struct phScene;

struct phConstraintsSolverInterface
{
	phScene* mScene;

//functions
	phConstraintsSolverInterface():mScene(0) {}
	phConstraintsSolverInterface(phScene* scene):mScene(scene) {}

	virtual void solveConstraints(float performance, float dt) {}
};

#endif //SOLVER_INTERFACE_H