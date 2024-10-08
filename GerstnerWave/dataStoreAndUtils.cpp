#include "dataStoreAndUtils.h"

dStoreAndUtils::dStoreAndUtils() {
    
}

void dStoreAndUtils::generateWaveField(vecTexCoord waveField[], unsigned int indOrder[]) {

	//Generate the first vertex of the field
	vecTexCoord vert = {
		glm::vec3(-3.2125f, 0.0f, 3.2125f),
		glm::vec2(0.0f, 0.0f)
	};

	int firstSquare[6] = {
		0, 1, 2049,
		2049, 2050, 1
	};

	//Doing the tesellation for the vector fields of the wave.
	int index = 0;
	int indRow = 0, indInd = 0;
	for (int i = 0; i < 2049; i++) {
		if(i == 1){
			indRow = 2049;
		}
		indRow = 2049 * i;

		for (int j = 0; j < 2049; j++) {

			glm::mat4 matTransform = glm::mat4(1.0f);
			glm::vec3 vectorTransform = glm::vec3((j * 0.1f), 0.0f, (i * -0.1f));
			glm::mat4 mat2DTransform = glm::mat4(1.0f);
			glm::vec3 vector2DTransform = glm::vec3((j * 0.0014245014f), (i * 0.0014245014f), 0.0f);
			matTransform = glm::translate(matTransform, vectorTransform);
			mat2DTransform = glm::translate(mat2DTransform, vector2DTransform);

			glm::vec3 result = matTransform * glm::vec4(vert.pos, 1.0f);
			glm::vec2 result2D = glm::vec2(mat2DTransform * glm::vec4(vert.texCoord, 0.0f, 1.0f));

			waveField[index].pos = result;
			waveField[index].texCoord = result2D;

			index++;

			if ((i < 2048) && (j < 2048)) {
				for (int k = 0; k < 6; k++) {
					indOrder[indInd] = firstSquare[k] + indRow + j;
					indInd++;
				}
			}
		}
	}



}

int dStoreAndUtils::loadSkybox(std::string mode) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	std::string texNames[] = { "right.png", "left.png", "up.png", "bottom.png", "behind.png", "front.png" };

	unsigned char* data[6];
	int width[6], height[6], nrChannels[6];

	for (int i = 0; i < 6; i++) {
		data[i] = stbi_load(("C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\" + mode + "-Cube-Map\\" + texNames[i]).c_str(), &width[i], &height[i], &nrChannels[i], 0);
		if (data[i] != NULL) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << ("C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\" + mode + "-Cube-Map\\" + texNames[i]).c_str() << std::endl;
		}
		stbi_image_free(data[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void dStoreAndUtils::loadWaveNRM(std::string* nrmNames, unsigned int* waveNRMID, int size) {

	for (int i = 0; i < size; i++) {
		glGenTextures(1, &waveNRMID[i]);
		glBindTexture(GL_TEXTURE_2D, waveNRMID[i]);

		unsigned char* data;
		int width, height, nrChannels;

		data = stbi_load(("C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\" + nrmNames[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Texture NRM failed to load at path: " << ("C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\" + nrmNames[i]).c_str() << std::endl;
		}
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

}

//Generating random directions of the wave each time and also the parameters
void dStoreAndUtils::generateDirectAndWaveParams(glm::vec2 directions[], glm::vec4 waveParams[], glm::vec3 linearInterpolators[], int sections[], int divisorLen, bool resDef) {

	if (!resDef) {
		srand(time(NULL));
		std::fstream curFileDirect, curFileWaveParam;
		curFileDirect.open("stdDefaultsDirect.txt", std::fstream::out);
		curFileWaveParam.open("stdDefaultsParam.txt", std::fstream::out);
		float deg;

		int ind = 0;
		for (int i = 0; i < divisorLen; i++) {

			for (int curPos = 0; curPos < sections[i]; curPos++) {

				deg = static_cast<float> (((rand() % 211) - 105) + ((static_cast <float> (rand())) / static_cast <float> (RAND_MAX)));
				std::cout << deg << std::endl;
				
				directions[ind] = glm::mat2(glm::vec2(glm::cos(glm::radians(deg)), glm::sin(glm::radians(deg))), glm::vec2(-glm::sin(glm::radians(deg)), glm::cos(glm::radians(deg)))) * glm::vec2(1.0, 1.0);
				curFileDirect << "defDirections[" << ind << "] = " << "glm::vec2(" << directions[ind].x << ", " << directions[ind].y << "),\n";

				waveParams[ind].x = std::lerp(linearInterpolators[(2 * i)].x, linearInterpolators[(2 * i) + 1].x, (static_cast<float> (curPos + 1) / sections[i]));
				waveParams[ind].y = std::lerp(linearInterpolators[(2 * i)].y, linearInterpolators[(2 * i) + 1].y, (static_cast<float> (curPos + 1) / sections[i]));

				waveParams[ind].z = sqrt(9.8 * waveParams[ind].y);
				waveParams[ind].w = linearInterpolators[(2 * i) + 1].z;

				curFileWaveParam << "defWaveParams[" << ind << "] = " << "glm::vec4(" << waveParams[ind].x << ", " << waveParams[ind].y << ", " << waveParams[ind].z << ", " << waveParams[ind].w << "),\n";
				ind++;
			}
		}

		curFileDirect.close();
		curFileDirect.close();
	}
	else {
		for (int i = 0; i < 64; i++) {
			directions[i] = defDirections[i];
			waveParams[i] = defWaveParams[i];
		}
	}

}