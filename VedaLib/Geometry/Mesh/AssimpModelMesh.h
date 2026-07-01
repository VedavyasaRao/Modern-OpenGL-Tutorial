#pragma once
#include "Base\GeometryMeshIndexed.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	Assimp::Importer importer; 
	const aiScene* scene = nullptr;
	aiNode* root_node = nullptr; 

	struct Mesh
	{
		vector<glm::vec3> vert_positions;
		vector<glm::vec3> vert_normals;
		vector<glm::vec2> tex_coords;
		vector<unsigned int> vert_indices;
		unsigned int tex_handle;
	};

	struct Texture
	{
		unsigned int textureID;
		string image_name;
	};

public:
	unsigned int num_meshes;
	vector<Mesh> mesh_list;
	vector<Texture> texture_list;

	Model(const char* model_path) 
	{
		scene = importer.ReadFile(model_path, aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
		load_model(); 
		dump_model(R"(c:\temp.txt)");
	}

private:
	void load_model()
	{
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			return;

		num_meshes = scene->mNumMeshes;
		mesh_list.resize(num_meshes);

		aiMesh* mesh{};
		int indices_offset = 0; 

		for (unsigned int i = 0; i < num_meshes; ++i)
		{
			mesh = scene->mMeshes[i]; 

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

			for (unsigned int tex_count = 0; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count) 
			{
				aiString string;
				material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string); 

				int already_loaded = is_image_loaded(string.C_Str()); 

				if (already_loaded == -1) 
				{
					bool load_success = false;
					unsigned int texture_handle = load_texture_image(string.C_Str(), load_success);

					if (load_success) 
					{
						Texture texture;
						texture.image_name = string.C_Str();
						texture.textureID = texture_handle;

						texture_list.push_back(texture);
						mesh_list[i].tex_handle = texture_handle;
					}
				}
				else
					mesh_list[i].tex_handle = already_loaded; 
			}

			for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
			{
				glm::vec3 position{};
				position.x = mesh->mVertices[i2].x;
				position.y = mesh->mVertices[i2].y;
				position.z = mesh->mVertices[i2].z;
				mesh_list[i].vert_positions.push_back(position);

				if (mesh->HasNormals())
				{
					glm::vec3 normal{};
					normal.x = mesh->mNormals[i2].x;
					normal.y = mesh->mNormals[i2].y;
					normal.z = mesh->mNormals[i2].z;
					mesh_list[i].vert_normals.push_back(normal);
				}
				else
					mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

				if (mesh->HasTextureCoords(0)) 
				{
					glm::vec2 tex_coords{};
					tex_coords.x = mesh->mTextureCoords[0][i2].x;
					tex_coords.y = mesh->mTextureCoords[0][i2].y;
					mesh_list[i].tex_coords.push_back(tex_coords);
				}
				else
					mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
			}

			for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
				for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
					mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);

			//set_buffer_data(i); 
		}
		
	}

	void dump_model(const string& outputfile)
	{
		ofstream ofs(outputfile.c_str());

		// Briefly looking at the node structure
		// ------------------------------------------------
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			ofs << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
		else
		{
			num_meshes = scene->mNumMeshes;
			mesh_list.resize(num_meshes);

			ofs << "\n\n   Start of Assimp Loading Meshes & Analysis";
			ofs << "\n   -----------------------------------------";

			root_node = scene->mRootNode;

			ofs << "\n   node->mNumMeshes: " << root_node->mNumMeshes;
			ofs << "\n   node->mName.C_Str(): " << root_node->mName.C_Str();
			ofs << "\n\n   node->mNumChildren: " << root_node->mNumChildren;
			// ------------------------------------------------------------------------------------------
			for (unsigned int i = 0; i < root_node->mNumChildren; ++i)
			{
				ofs << "\n   node->mChildren[i]->mName.C_Str(): " << root_node->mChildren[i]->mName.C_Str();
				ofs << "\n   node->mChildren[i]->mNumMeshes: " << root_node->mChildren[i]->mNumMeshes;
			}
			ofs << "\n\n   scene->HasMaterials(): " << scene->HasMaterials();
			// ------------------------------------------------------------------------------------------
			for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
				ofs << "\n   scene->mMaterials[i]->GetName(): " << scene->mMaterials[i]->GetName().C_Str();

			ofs << "\n\n   scene->HasTextures(): " << scene->HasTextures();

			aiMesh* mesh{};

			int total_num_indices = 0;
			int indices_offset = 0; // Not being used yet... i.e. indices_offset += mesh->mNumVertices; is commented further down.

			// (1) Loop through all the model's meshes
			// -----------------------------------------------------
			ofs << "\n   scene->mNumMeshes: " << num_meshes;
			ofs << "\n   ********************\n";
			// ---------------------------------------------------------
			for (unsigned int i = 0; i < num_meshes; ++i) // In this case... scene->mNumMeshes = node->mChildren[i]->mNumMeshes
			{
				mesh = scene->mMeshes[i]; // http://assimp.sourceforge.net/lib_html/structai_mesh.html

				ofs << "\n\n   mesh->mMaterialIndex: " << mesh->mMaterialIndex;
				ofs << "\n   ----------------------- ";
				ofs << "\n   mesh->mName.C_Str(): " << mesh->mName.C_Str();

				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // http://assimp.sourceforge.net/lib_html/structai_material.html

				ofs << "\n\n   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string): " << material->GetTextureCount(aiTextureType_DIFFUSE);
				ofs << "\n   material->GetTexture(aiTextureType_SPECULAR, tex_count, &string): " << material->GetTextureCount(aiTextureType_SPECULAR);
				ofs << "\n   material->GetTexture(aiTextureType_AMBIENT, tex_count, &string): " << material->GetTextureCount(aiTextureType_AMBIENT) << "\n\n";

				unsigned int tex_count = 0;
				for (; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count) // The above ofs reveals that only using: aiTextureType_DIFFUSE
				{
					aiString astring;
					material->GetTexture(aiTextureType_DIFFUSE, tex_count, &astring); // Acquire the name of the image file to be loaded.
					ofs << "   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &astring): " << astring.C_Str() << "\n\n";

					// (2) Load mesh [i]'s texture if not already loaded
					// ---------------------------------------------------------------
					int already_loaded = is_image_loaded(astring.C_Str()); // Returns -1 if texture Not already loaded, otherwise returns Existing texture handle.
					ofs << "   Loading Image\n";

					if (already_loaded == -1) // Image not yet loaded.
					{
						bool load_complete = false;
						unsigned int texture_handle = load_texture_image(astring.C_Str(), load_complete);

						if (load_complete) // Although do nothing if the image fails to load.
						{
							Texture texture;
							texture.image_name = astring.C_Str();
							texture.textureID = texture_handle;

							texture_list.push_back(texture);
							mesh_list[i].tex_handle = texture_handle;
						}
					}
					else // Assign existing texture handle.
					{
						string edited = astring.C_Str();
						size_t position = edited.find_last_of("\\");

						ofs << "   Image file: " << edited.substr(position + 1) << " (is already loaded)";
						mesh_list[i].tex_handle = already_loaded;
					}
				}
				if (tex_count == 0)
					ofs << "   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string): No image has been applied to this mesh\n\n";
				else
					ofs << "\n";

				for (unsigned int slot = 0; slot < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++slot)
					ofs << "   mesh->HasTextureCoords(" << slot << "): " << mesh->HasTextureCoords(slot) << "\n";

				ofs << "\n   Mesh index: " << i << " (mesh->mNumVertices: " << mesh->mNumVertices << ")";
				ofs << "\n   ------------------------------------- ";

				// (3) Loop through all mesh [i]'s vertices
				// ---------------------------------------------------
				for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
				{
					glm::vec3 position{};
					position.x = mesh->mVertices[i2].x;
					position.y = mesh->mVertices[i2].y;
					position.z = mesh->mVertices[i2].z;
					mesh_list[i].vert_positions.push_back(position);

					ofs << "\n   Count: " << i2;
					ofs << "\n   mesh->mVertices[" << i2 << "].x: " << position.x;
					ofs << "\n   mesh->mVertices[" << i2 << "].y: " << position.y;
					ofs << "\n   mesh->mVertices[" << i2 << "].z: " << position.z;

					if (mesh->HasNormals())
					{
						glm::vec3 normal{};
						normal.x = mesh->mNormals[i2].x;
						normal.y = mesh->mNormals[i2].y;
						normal.z = mesh->mNormals[i2].z;
						mesh_list[i].vert_normals.push_back(normal);
						ofs << "\n   mesh->mNormals[" << i2 << "] X: " << normal.x << " Y: " << normal.y << " Z: " << normal.z;
					}
					else
						mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

					if (mesh->HasTextureCoords(0)) // Above for loop: AI_MAX_NUMBER_OF_TEXTURECOORDS reveals that only slot [0] is in question.
					{
						glm::vec2 tex_coords{};
						tex_coords.x = mesh->mTextureCoords[0][i2].x;
						tex_coords.y = mesh->mTextureCoords[0][i2].y;
						mesh_list[i].tex_coords.push_back(tex_coords);
						ofs << "\n   mesh->mTextureCoords[0][" << i2 << "] X: " << tex_coords.x << " Y: " << tex_coords.y;
					}
					else
						mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
				}
				ofs << "\n\n   mesh->mNumFaces: " << mesh->mNumFaces << "\n";
				ofs << "   ------------------ ";

				for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
				{
					ofs << "\n";
					for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
					{
						ofs << "   mesh->mFaces[" << i3 << "].mIndices[" << i4 << "]: " << mesh->mFaces[i3].mIndices[i4] << "\n";
						mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);
						++total_num_indices;
					}
				}
				ofs << "\n   Total number of indices: " << total_num_indices;
				ofs << "\n   **************************";
				total_num_indices = 0;

				ofs << "\n   Indices offset (Total 'mesh->mNumVertices' so far): " << indices_offset;
				ofs << "\n   *****************************************************\n\n";

			}
			if (texture_list.size() > 0)
				for (unsigned int i = 0; i < texture_list.size(); ++i)
				{
					ofs << "   image_list[" << i << "].imageID: " << texture_list[i].textureID << "... image_list[" << i << "].image_name: " << texture_list[i].image_name << "\n";

					for (unsigned int i2 = 0; i2 < num_meshes; ++i2)
						if (texture_list[i].textureID == mesh_list[i2].tex_handle)
							ofs << "   mesh_list[" << i2 << "].tex_handle: " << mesh_list[i2].tex_handle << "\n";
					ofs << "\n";
				}
			else
				ofs << "   ***** No images have been loaded\n";
		}
	}

	//void set_buffer_data(unsigned int index)
	//{
	//	glGenVertexArrays(1, &mesh_list[index].VAO);
	//	glGenBuffers(1, &mesh_list[index].VBO1); // Alternative to using 3 separate VBOs, instead use only 1 VBO and set glVertexAttribPointer's offset...
	//	glGenBuffers(1, &mesh_list[index].VBO2); // like was done in tutorial 3... Orbiting spinning cubes.
	//	glGenBuffers(1, &mesh_list[index].VBO3);
	//	glGenBuffers(1, &mesh_list[index].EBO);

	//	glBindVertexArray(mesh_list[index].VAO);

	//	// Vertex Positions
	//	// ---------------------
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO1);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_positions.size(), &mesh_list[index].vert_positions[0], GL_STATIC_DRAW);

	//	glEnableVertexAttribArray(0); // Void pointer below is for legacy reasons. Two possible meanings: "offset for buffer objects" & "address for client state arrays"
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//	// Vertex Normals
	//	// --------------------
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO2);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_normals.size(), &mesh_list[index].vert_normals[0], GL_STATIC_DRAW);

	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//	// Texture Coordinates
	//	// ---------------------------
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO3);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh_list[index].tex_coords.size(), &mesh_list[index].tex_coords[0], GL_STATIC_DRAW);

	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//	// Indices for: glDrawElements()
	//	// ---------------------------------------
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);

	//	glBindVertexArray(0); 	// Unbind VAO
	//}

	int is_image_loaded(string file_name)
	{
		for (unsigned int i = 0; i < texture_list.size(); ++i)
			if (file_name.compare(texture_list[i].image_name) == 0)
				return texture_list[i].textureID;
		return -1;
	}

	unsigned int load_texture_image(string file_name, bool& load_complete)
	{
	//	// stbi_set_flip_vertically_on_load(1); // Call this function if the image is upside-down.		

	//	size_t position = file_name.find_last_of("\\");
	//	file_name = "Images\\" + file_name.substr(position + 1);

	//	int width, height, num_components;
	//	unsigned char* image_data = stbi_load(file_name.c_str(), &width, &height, &num_components, 0);

	//	unsigned int textureID;
	//	glGenTextures(1, &textureID);

	//	if (image_data)
	//	{
	//		GLenum format{};

	//		if (num_components == 1)
	//			format = GL_RED;
	//		else if (num_components == 3)
	//			format = GL_RGB;
	//		else if (num_components == 4)
	//			format = GL_RGBA;

	//		glBindTexture(GL_TEXTURE_2D, textureID);
	//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Recommended by NVIDIA Rep: https://devtalk.nvidia.com/default/topic/875205/opengl/how-does-gl_unpack_alignment-work-/

	//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
	//		glGenerateMipmap(GL_TEXTURE_2D);

	//		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	//		// ----------------------------------------------------------------------------------------------------------------
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // GL_REPEAT... GL_MIRRORED_REPEAT... GL_CLAMP_TO_EDGE... GL_CLAMP_TO_BORDER.
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//		// float border_colour[] = {0.45, 0.55, 0.95};
	//		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour); // For above when using: GL_CLAMP_TO_BORDER		

	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST... GL_LINEAR... GL_NEAREST_MIPMAP_NEAREST (See above link for full list)
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST or GL_LINEAR.

	//		load_complete = true;
	//		stbi_image_free(image_data);
	//		cout << "   Image loaded OK: " << file_name << "\n";
	//	}
	//	else
	//	{
	//		load_complete = false;
	//		stbi_image_free(image_data);
	//		cout << "   Image failed to load: " << file_name << "\n";
	//	}
	//	return textureID;
		return 0;
	}
};


//Implements IGeometryMesh for Cube object
class AssimpModelMesh :public IGeometryMesh
{
public:
	//
	AssimpModelMesh(const string& objfilename)
	{
		Model m(objfilename.c_str());

		//LoadOBJMeshs(objfilename);
	}

	//Generate VBO vertex data in non Indexed Mode 
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		for (unsigned short i = 0; i < faces.size(); ++i)
		{
			auto vtx = faces[i];
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(vertices[vtx[0]]);
			}

			if (att & VAOUtil::TEX)
			{
				if (vtx[1] != -1)
					vaoutl.AddTexCord(texturemap[vtx[1]]);
			}

			if (att & VAOUtil::NOR)
			{
				if (vtx[2] != -1)
					vaoutl.AddNormal(normals[vtx[2]]);
			}
		}
		return faces.size();
	}

	inline uint vertex_count() const
	{
		return vertices.size();
	}

	inline uint texture_count() const
	{
		return texturemap.size();
	}

	inline uint normals_count() const
	{
		return normals.size();
	}


	bool LoadOBJMeshs(const string& filePath)
	{
		// Initialize the Assimp Importer
		Assimp::Importer importer;

		// Read the file with post-processing flags
		const aiScene* scene = importer.ReadFile(filePath,
			aiProcess_Triangulate |           // Force all polygons into triangles
			aiProcess_FlipUVs |               // Flip texture coordinates on the Y-axis (crucial for OpenGL)
			aiProcess_GenSmoothNormals |      // Generate smooth normals if missing
			aiProcess_JoinIdenticalVertices   // Optimize memory by indexing shared vertices
		);

		// Check for errors during parsing
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return false;
		}

		// Successfully loaded, proceed to parse the node structure
		cout << "Successfully loaded: " << filePath << endl;

		// Start processing your model data starting from the root node
		// ProcessNode(scene->mRootNode, scene);

		return true;
	}

	void ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Position coordinates
			float posX = mesh->mVertices[i].x;
			float posY = mesh->mVertices[i].y;
			float posZ = mesh->mVertices[i].z;

			// Normals (Ensure they exist first)
			if (mesh->HasNormals())
			{
				float normX = mesh->mNormals[i].x;
				// ...
			}

			// Texture Coordinates (Assimp supports up to 8 channels; OBJ uses channel 0)
			if (mesh->mTextureCoords[0])
			{
				float u = mesh->mTextureCoords[0][i].x;
				float v = mesh->mTextureCoords[0][i].y;
			}
		}
	}

public:

	vector <glm::vec3>		vertices;
	vector <glm::vec2>		texturemap;
	vector <glm::vec3>		normals;
	vector <glm::i16vec3>	faces;


private:
	string text;
};
