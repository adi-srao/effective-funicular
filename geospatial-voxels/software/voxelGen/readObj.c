#include "voxelGen.h"

int readObj(char *fileName, Vertex **_Vertices, int *_numV, Face **_Faces, int *_numF) {
    int numV = 0, numVN = 0, numVT = 0, numF = 0;
    FILE *fpIN = NULL;
    char lineBuffer[BUFSIZ];
    Vertex *Vertices = NULL;
    Face *Faces = NULL;

    /*Open file*/
    fpIN = fopen(fileName, "r");
    if (!fpIN) {
        fprintf(stderr, "ERROR: the path for file containing the input files is invalid %s\n", fileName);
        return -1;
    }

    /*Count the number of vertexes and faces*/
    while(fgets(lineBuffer, sizeof(lineBuffer), fpIN)) {
        lineBuffer[strlen(lineBuffer)-1]='\0';
        if (lineBuffer[0] == 'v') {
            if (lineBuffer[1] == ' ')
                numV++;
            if (lineBuffer[1] == 't')
                numVT++;
            if (lineBuffer[1] == 'n')
                numVN++;
        }
        if (lineBuffer[0] == 'f')
            numF++;
    }
    (void) fseek(fpIN, 0, SEEK_SET);

    /*Allocate the lists*/
    Vertices = (Vertex*) malloc(sizeof(Vertex)*numV);
    Faces = (Face*) malloc(sizeof(Face)*numF);

    /*Read Faces and Vertexes*/
    numV = 0;
    numF = 0;
    while(fgets(lineBuffer, sizeof(lineBuffer), fpIN)) {
        int res = 0;
        lineBuffer[strlen(lineBuffer)-1]='\0';
        if (lineBuffer[0] == 'v') {
            if (lineBuffer[1] == ' ') {
                if ( (res = sscanf(lineBuffer,"v%*[ ]%lf%*[ ]%lf%*[ ]%lf",&(Vertices[numV].x),&(Vertices[numV].y),&(Vertices[numV].z))) != 3) {
                    fprintf(stderr, "ERROR: It failed to parse %s\n",lineBuffer);
                    return -1;
                }
                numV++;
            }
        }
        if (lineBuffer[0] == 'f') {
            int p1 = 0, p2 = 0, p3 = 0;
            char v1[64], v2[64], v3[64];
            res = 0;
            
            // Parse face line flexibly - handle "f v1 v2 v3" or "f v1/... v2/... v3/..."
            res = sscanf(lineBuffer, "f %s %s %s", v1, v2, v3);
            
            if (res == 3) {
                // Extract just the vertex index (first number before any '/')
                p1 = atoi(v1);
                p2 = atoi(v2);
                p3 = atoi(v3);
                
                // Validate indices
                if (p1 < 1 || p1 > numV || p2 < 1 || p2 > numV || p3 < 1 || p3 > numV) {
                    fprintf(stderr, "ERROR: Invalid vertex indices in face: %s (vertices available: %d)\n", lineBuffer, numV);
                    return -1;
                }
                
                //printf ("f %d %d %d\n", p1, p2, p3);
                Faces[numF].p1 = &Vertices[p1-1];
                Faces[numF].p2 = &Vertices[p2-1];
                Faces[numF].p3 = &Vertices[p3-1];
                numF++;
            } else {
                fprintf(stderr, "ERROR: It failed to parse %s (got %d vertex refs, expected 3)\n", lineBuffer, res);
                return -1;
            }
        }
    }
    fclose(fpIN);
    *_numF = numF;
    *_numV = numV;
    *_Vertices = Vertices;
    *_Faces = Faces;

    return 0;
}