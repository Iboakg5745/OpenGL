#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include <glutils.hpp>
#include <camera.hpp>
#include <renderer.hpp>
#include <loadObj.hpp>

void loadObj(const std::string objPath, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	int totalVertices = 0;

	FILE* obj = fopen(objPath.c_str(), "r");
	if (obj) {
		while (true) {
			char strRead[200];

			int nbArgs = fscanf(obj, "%s", strRead);
			if (nbArgs == EOF) {
				std::cout << "End of file reached" << std::endl;
				break;
			}

			// Parse total number of vertices
			if (std::strcmp(strRead, "#") == 0) {
				int temp_nb = 0;
				if (fscanf(obj, "%i %s\n", &temp_nb, strRead) != 2) {
					if (std::strcmp(strRead, "vertices") == 0) {
						std::cout << "Error encountered while parsing total vertices" << std::endl;
					}
				}
				else {
					if (std::strcmp(strRead, "vertices") == 0) {
						totalVertices = temp_nb;
					}
				}
			}

			// Parse vertex, v = vertex
			if (std::strcmp(strRead, "v") == 0) {
				glm::vec3 temp_vertex = glm::vec3(0.0, 0.0, 0.0);

				if (fscanf(obj, "%f %f %f\n", &temp_vertex.x, &temp_vertex.y, &temp_vertex.z) != 3) {
					std::cout << "Error encountered while parsing a vertex" << std::endl;
					break;
				}
				else {
					temp_vertices.push_back(temp_vertex);
				}
			}

			// Parse texture coordinate of vertex, vt = vertex texture
			if (std::strcmp(strRead, "vt") == 0) {
				glm::vec2 temp_uv = glm::vec2(0.0, 0.0);

				if (fscanf(obj, "%f %f \n", &temp_uv.x, &temp_uv.y) != 2) {
					std::cout << "Error encountered while parsing a texture coordinate" << std::endl;
					break;
				}
				else {
					temp_uvs.push_back(temp_uv);
				}
			}

			// Parse normals, vn = vertex normal
			if (std::strcmp(strRead, "vn") == 0) {
				glm::vec3 temp_normal = glm::vec3(0.0, 0.0, 0.0);

				if (fscanf(obj, "%f %f %f\n", &temp_normal.x, &temp_normal.y, &temp_normal.z) != 3) {
					std::cout << "Error encountered while parsing a vertex normal" << std::endl;
					break;
				}
				else {
					temp_normals.push_back(temp_normal);
				}
			}

			// Parse vertex face, f = face
			if (std::strcmp(strRead, "f") == 0) {
				std::string tmp_vtx1, tmp_vtx2, tmp_vtx3;
				unsigned int vtxIdx[3], uvIdx[3], normalIdx[3];

				if (fscanf(obj, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vtxIdx[0], &uvIdx[0], &normalIdx[0], &vtxIdx[1], &uvIdx[1], &normalIdx[1], &vtxIdx[2], &uvIdx[2], &normalIdx[2]) != 9) {
					std::cout << "Error encountered while parsing a vertex face" << std::endl;
					break;
				}
				else {
					vertexIndices.push_back(vtxIdx[0]);
					vertexIndices.push_back(vtxIdx[1]);
					vertexIndices.push_back(vtxIdx[2]);

					uvIndices.push_back(uvIdx[0]);
					uvIndices.push_back(uvIdx[1]);
					uvIndices.push_back(uvIdx[2]);

					normalIndices.push_back(normalIdx[0]);
					normalIndices.push_back(normalIdx[1]);
					normalIndices.push_back(normalIdx[2]);
				}
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];

			out_vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < uvIndices.size(); i++) {
			glm::vec2 uv = temp_uvs[uvIndices[i] - 1];

			out_uvs.push_back(uv);
		}

		for (unsigned int i = 0; i < normalIndices.size(); i++) {
			glm::vec3 normal = temp_normals[normalIndices[i] - 1];

			out_normals.push_back(normal);
		}
	}
}