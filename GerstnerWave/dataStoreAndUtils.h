#ifndef DATASTOREANDUTILS_H
#define DATASTOREANDUTILS_H

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "stb_image.h"
#include "Shader.h"

class dStoreAndUtils {
private:
    //dataStoreAndUtils properties
    glm::vec2 defDirections[64] = {
        defDirections[0] = glm::vec2(-0.0456746, 1.41348),
        defDirections[1] = glm::vec2(0.421545, 1.34993),
        defDirections[2] = glm::vec2(1.41355, 0.0434566),
        defDirections[3] = glm::vec2(1.41416, 0.0127038),
        defDirections[4] = glm::vec2(0.0453592, 1.41349),
        defDirections[5] = glm::vec2(0.916831, 1.07676),
        defDirections[6] = glm::vec2(0.700955, 1.22828),
        defDirections[7] = glm::vec2(1.11594, 0.868724),
        defDirections[8] = glm::vec2(1.22828, 0.70094),
        defDirections[9] = glm::vec2(1.40274, -0.179813),
        defDirections[10] = glm::vec2(1.1554, 0.815512),
        defDirections[11] = glm::vec2(0.919377, 1.07459),
        defDirections[12] = glm::vec2(1.38719, 0.275149),
        defDirections[13] = glm::vec2(1.18536, 0.771307),
        defDirections[14] = glm::vec2(0.758339, 1.1937),
        defDirections[15] = glm::vec2(1.41052, 0.102149),
        defDirections[16] = glm::vec2(1.39687, 0.220782),
        defDirections[17] = glm::vec2(1.40924, -0.118539),
        defDirections[18] = glm::vec2(0.79402, 1.17027),
        defDirections[19] = glm::vec2(0.976947, 1.02253),
        defDirections[20] = glm::vec2(-0.216053, 1.39761),
        defDirections[21] = glm::vec2(0.310674, 1.37967),
        defDirections[22] = glm::vec2(0.776632, 1.18188),
        defDirections[23] = glm::vec2(1.41323, 0.05281),
        defDirections[24] = glm::vec2(1.3985, -0.210246),
        defDirections[25] = glm::vec2(0.194982, 1.40071),
        defDirections[26] = glm::vec2(0.0171661, 1.41411),
        defDirections[27] = glm::vec2(0.834565, 1.14171),
        defDirections[28] = glm::vec2(0.530506, 1.31094),
        defDirections[29] = glm::vec2(0.849629, 1.13054),
        defDirections[30] = glm::vec2(0.783116, 1.17759),
        defDirections[31] = glm::vec2(1.1246, 0.857478),
        defDirections[32] = glm::vec2(0.804896, 1.16282),
        defDirections[33] = glm::vec2(1.41098, 0.0956078),
        defDirections[34] = glm::vec2(0.713705, 1.22091),
        defDirections[35] = glm::vec2(0.229181, 1.39552),
        defDirections[36] = glm::vec2(0.893383, 1.0963),
        defDirections[37] = glm::vec2(1.36777, 0.359468),
        defDirections[38] = glm::vec2(1.3079, 0.537948),
        defDirections[39] = glm::vec2(1.37837, -0.316359),
        defDirections[40] = glm::vec2(0.430569, 1.34707),
        defDirections[41] = glm::vec2(1.1013, 0.887208),
        defDirections[42] = glm::vec2(1.4127, 0.0654056),
        defDirections[43] = glm::vec2(1.39065, 0.257092),
        defDirections[44] = glm::vec2(0.0369253, 1.41373),
        defDirections[45] = glm::vec2(1.04379, 0.954204),
        defDirections[46] = glm::vec2(1.39863, -0.20939),
        defDirections[47] = glm::vec2(0.912101, 1.08077),
        defDirections[48] = glm::vec2(1.40279, -0.179383),
        defDirections[49] = glm::vec2(0.704457, 1.22627),
        defDirections[50] = glm::vec2(0.353744, 1.36926),
        defDirections[51] = glm::vec2(1.41401, -0.0242258),
        defDirections[52] = glm::vec2(1.31695, 0.515416),
        defDirections[53] = glm::vec2(-0.300883, 1.38184),
        defDirections[54] = glm::vec2(0.873559, 1.11216),
        defDirections[55] = glm::vec2(1.37942, -0.311754),
        defDirections[56] = glm::vec2(0.371591, 1.36452),
        defDirections[57] = glm::vec2(1.40298, -0.177865),
        defDirections[58] = glm::vec2(-0.375998, 1.36331),
        defDirections[59] = glm::vec2(1.32595, 0.491801),
        defDirections[60] = glm::vec2(0.526675, 1.31248),
        defDirections[61] = glm::vec2(0.380158, 1.36216),
        defDirections[62] = glm::vec2(1.39712, 0.219241),
        defDirections[63] = glm::vec2(1.06204, 0.933845)
    };

    glm::vec4 defWaveParams[64]{
        defWaveParams[0] = glm::vec4(0.0105, 2.08319, 4.51832, 2),
        defWaveParams[1] = glm::vec4(0.010371, 2.15093, 4.5912, 2),
        defWaveParams[2] = glm::vec4(0.0102419, 2.21867, 4.66293, 2),
        defWaveParams[3] = glm::vec4(0.0101129, 2.28641, 4.73359, 2),
        defWaveParams[4] = glm::vec4(0.00998387, 2.35415, 4.8032, 2),
        defWaveParams[5] = glm::vec4(0.00985484, 2.42189, 4.87181, 2),
        defWaveParams[6] = glm::vec4(0.00972581, 2.48964, 4.93948, 2),
        defWaveParams[7] = glm::vec4(0.00959677, 2.55738, 5.00623, 2),
        defWaveParams[8] = glm::vec4(0.00946774, 2.62512, 5.0721, 2),
        defWaveParams[9] = glm::vec4(0.00933871, 2.69286, 5.13713, 2),
        defWaveParams[10] = glm::vec4(0.00920968, 2.7606, 5.20134, 2),
        defWaveParams[11] = glm::vec4(0.00908064, 2.82835, 5.26477, 2),
        defWaveParams[12] = glm::vec4(0.00895161, 2.89609, 5.32744, 2),
        defWaveParams[13] = glm::vec4(0.00882258, 2.96383, 5.38939, 2),
        defWaveParams[14] = glm::vec4(0.00869355, 3.03157, 5.45063, 2),
        defWaveParams[15] = glm::vec4(0.00856452, 3.09931, 5.5112, 2),
        defWaveParams[16] = glm::vec4(0.00843548, 3.16706, 5.5711, 2),
        defWaveParams[17] = glm::vec4(0.00830645, 3.2348, 5.63037, 2),
        defWaveParams[18] = glm::vec4(0.00817742, 3.30254, 5.68901, 2),
        defWaveParams[19] = glm::vec4(0.00804839, 3.37028, 5.74707, 2),
        defWaveParams[20] = glm::vec4(0.00791936, 3.43802, 5.80454, 2),
        defWaveParams[21] = glm::vec4(0.00779032, 3.50577, 5.86144, 2),
        defWaveParams[22] = glm::vec4(0.00766129, 3.57351, 5.9178, 2),
        defWaveParams[23] = glm::vec4(0.00753226, 3.64125, 5.97363, 2),
        defWaveParams[24] = glm::vec4(0.00740323, 3.70899, 6.02894, 2),
        defWaveParams[25] = glm::vec4(0.00727419, 3.77673, 6.08375, 2),
        defWaveParams[26] = glm::vec4(0.00714516, 3.84448, 6.13807, 2),
        defWaveParams[27] = glm::vec4(0.00701613, 3.91222, 6.19191, 2),
        defWaveParams[28] = glm::vec4(0.0068871, 3.97996, 6.24529, 2),
        defWaveParams[29] = glm::vec4(0.00675806, 4.0477, 6.29821, 2),
        defWaveParams[30] = glm::vec4(0.00662903, 4.11544, 6.3507, 2),
        defWaveParams[31] = glm::vec4(0.0065, 4.18319, 6.40275, 2),
        defWaveParams[32] = glm::vec4(0.125, 0.531853, 2.28302, 1),
        defWaveParams[33] = glm::vec4(0.11875, 0.539624, 2.29963, 1),
        defWaveParams[34] = glm::vec4(0.1125, 0.547395, 2.31613, 1),
        defWaveParams[35] = glm::vec4(0.10625, 0.555166, 2.33251, 1),
        defWaveParams[36] = glm::vec4(0.1, 0.562937, 2.34878, 1),
        defWaveParams[37] = glm::vec4(0.09375, 0.570707, 2.36494, 1),
        defWaveParams[38] = glm::vec4(0.0875, 0.578478, 2.38098, 1),
        defWaveParams[39] = glm::vec4(0.08125, 0.586249, 2.39692, 1),
        defWaveParams[40] = glm::vec4(0.075, 0.59402, 2.41276, 1),
        defWaveParams[41] = glm::vec4(0.06875, 0.601791, 2.42849, 1),
        defWaveParams[42] = glm::vec4(0.0625, 0.609562, 2.44412, 1),
        defWaveParams[43] = glm::vec4(0.05625, 0.617333, 2.45965, 1),
        defWaveParams[44] = glm::vec4(0.05, 0.625103, 2.47508, 1),
        defWaveParams[45] = glm::vec4(0.04375, 0.632874, 2.49042, 1),
        defWaveParams[46] = glm::vec4(0.0375, 0.640645, 2.50566, 1),
        defWaveParams[47] = glm::vec4(0.03125, 0.648416, 2.52081, 1),
        defWaveParams[48] = glm::vec4(0.025, 0.656187, 2.53587, 1),
        defWaveParams[49] = glm::vec4(0.01875, 0.663958, 2.55084, 1),
        defWaveParams[50] = glm::vec4(0.0125, 0.671729, 2.56572, 1),
        defWaveParams[51] = glm::vec4(0.00625001, 0.6795, 2.58052, 1),
        defWaveParams[52] = glm::vec4(7.45058e-09, 0.68727, 2.59524, 1),
        defWaveParams[53] = glm::vec4(-0.00624999, 0.695041, 2.60987, 1),
        defWaveParams[54] = glm::vec4(-0.0125, 0.702812, 2.62442, 1),
        defWaveParams[55] = glm::vec4(-0.01875, 0.710583, 2.63889, 1),
        defWaveParams[56] = glm::vec4(-0.025, 0.718354, 2.65328, 1),
        defWaveParams[57] = glm::vec4(-0.03125, 0.726125, 2.66759, 1),
        defWaveParams[58] = glm::vec4(-0.0375, 0.733896, 2.68182, 1),
        defWaveParams[59] = glm::vec4(-0.04375, 0.741666, 2.69598, 1),
        defWaveParams[60] = glm::vec4(-0.05, 0.749437, 2.71007, 1),
        defWaveParams[61] = glm::vec4(-0.05625, 0.757208, 2.72409, 1),
        defWaveParams[62] = glm::vec4(-0.0625, 0.764979, 2.73803, 1),
        defWaveParams[63] = glm::vec4(-0.06875, 0.77275, 2.7519, 1)
    };

public:
    //Publicly Available Data
    typedef struct vecAndTexCoord {
        glm::vec3 pos;
        glm::vec2 texCoord;
    } vecTexCoord;

    float skyboxVertices[108] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // constructor
    dStoreAndUtils();

    // utility functions
    void generateWaveField(vecTexCoord waveField[], unsigned int indOrder[]);
    int loadSkybox(std::string mode);
    void generateDirectAndWaveParams(glm::vec2 directions[], glm::vec4 waveParams[], glm::vec3 linearInterpolators[], int sections[], int divisorLen, bool resDef);
    void loadWaveNRM(std::string *nrmNames, unsigned int *waveNRMID, int size);

    //Getter and Setter

};

#endif
