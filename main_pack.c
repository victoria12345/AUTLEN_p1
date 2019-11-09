#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pack.h"

int main(int argc, char ** argv)
{
    Pack *p1, *p2, *p3;
    Pack** packs;
    int e1[3] = {1,0,0}, e2[3] = {0,1,0};

    packs = (Pack**)malloc(sizeof(Pack*) * 2);

    p1 = pack(e1,3,"lolo");
    p2 = pack(e2,3,"s");
    p3 = pack(e2,3, "lolo");

    packs[0] = p1;
    packs[1] = p2;
    printf("p3 no pertenece a packs: %d\n", pertenece(packs, p3,2));
    printf("p2 si pertenece a packs: %d\n", pertenece(packs, p2,2));

    printf("Resultado de comparar p1 y p2 (-1): %d\n", comparar(p1,p2));
    printf("Resultado de comparar p2 y p3(-0); %d\n", comparar(p2,p3));

    printf("nombres: %s\n%s\n%s\n", get_nombre(p1), get_nombre(p2), get_nombre(p3));

    destroy(p1);
    destroy(p2);
    destroy(p3);
	return 0;
}
