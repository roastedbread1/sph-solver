#include "model.h"

void LoadModel(Model* model, std::string path)
{

	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "error::assimp" << import.GetErrorString() << std::endl;
		return;
	}
	model->directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(model, scene->mRootNode, scene);

}

void ProcessNode(Model* model, aiNode* node, const aiScene* scene)
{	//process all node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		///place an empty mesh first and then modify it
		model->meshes.emplace_back();
		ProcessMesh(model->directory, &model->meshes.back(), mesh, scene);
	}
	//then process its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(model, node->mChildren[i], scene);
	}
}

void ProcessMesh(std::string directory, Mesh* mesh, aiMesh* ai_mesh, const aiScene* scene)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
	{
		Vertex vertex;
		///process vertex positions, normals, and texture coordinates

		///needed because assimp maintains its own data types
		glm::vec3 vector;
		vector.x = ai_mesh->mVertices[i].x;
		vector.y = ai_mesh->mVertices[i].y;
		vector.z = ai_mesh->mVertices[i].z;
		vertex.position = vector;

		if (ai_mesh->HasNormals())
		{
			vector.x = ai_mesh->mNormals[i].x;
			vector.y = ai_mesh->mNormals[i].y;
			vector.z = ai_mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (ai_mesh->HasTextureCoords(0))
		{
			glm::vec2 vec;

			vec.x = ai_mesh->mTextureCoords[0][i].x;
			vec.y = ai_mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}

		vertices.emplace_back(vertex);
	}
	///process indices

	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace face = ai_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}


	///process material
	if (ai_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(directory, material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(directory, material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	initMesh(mesh, vertices, indices, textures);
}

std::vector<Texture> LoadMaterialTextures(std::string directory, aiMaterial* mat, aiTextureType type, std::string typeName)
{

	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (size_t j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)

			{
				textures.emplace_back(textures_loaded[i]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, false);
			texture.type = typeName;
			texture.path = std::string(str.C_Str());
			textures.emplace_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

void DrawModel(Model* model, Shader& shader)
{
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		DrawMesh(&model->meshes[i], shader);
	}
}

void DrawInstancedModel(Model* model, Shader& shader, int instanceCount)
{
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		DrawInstancedMesh(&model->meshes[i], shader, instanceCount);
	}
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{

	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "texture failed to load at path: " << path << "\n";
	}

	return textureID;
}
