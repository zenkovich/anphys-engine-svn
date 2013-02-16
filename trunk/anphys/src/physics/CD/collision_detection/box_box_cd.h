#ifndef BOX_BOX_CD_H
#define BOX_BOX_CD_H

struct phCollision;
struct phBoxCollisionGeometry;

phCollision* checkCollisionBoxBox(phBoxCollisionGeometry* geomA, phBoxCollisionGeometry* geomB, phCollision* collision);

#endif //BOX_BOX_CD_H