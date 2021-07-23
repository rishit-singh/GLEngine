#ifndef RENDERER_H

#include "window.h"
#include "vertexarray.h"
#include "texture.h"

namespace GLEngine
{
	class Mesh	//	3D/2D Mesh
	{
	private:
		void DeleteVAOs();	//	Deletes all the allocated VAOs in the current mesh instance.

	public:
		int MatrixSize;	//	VertexMatrix array length

		std::vector<VertexArrayObject*> VertexArrayObjects;	// Stores all the vertex array objects 
		
		std::vector<Vertex3Df> VertexMatrixVector;	// Stores the triangle vertex matrix of a maesh

		Shader* MeshShader;	// Shader object of the current mesh
	
		Texture* MeshTexture;	//	Texture to be mapped on the mesh objects.

		float* VertexMatrixArray;

		void Update(unsigned int);	//	Recompiles all the shaders and resets the VAO at the provided index
		
		bool AddVertexArrayObject(VertexArrayObject*);	//	Adds the provided VertexArrayObject to the current mesh.
		bool AddBufferObject(VertexBufferObject, unsigned int);	//	Adds the provided VertexBufferObject to the VertexArrayObject at the provided index.

		bool IsValid();
		
		Mesh() : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(nullptr), MatrixSize(0), MeshShader(nullptr), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)
		{
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)
		{	
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));
			
			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();
			
			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));
	
			if (this->MeshTexture->IsValid())
				this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));

			this->VertexArrayObjects.back()->SetVertexAttributePointer();
			
			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)		
		{			
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));

			this->VertexArrayObjects.back()->SetVertexAttributePointer();
		}

		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture)
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			bool valid;

			if (valid = this->MeshTexture->IsValid())
				this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(std::vector<Vertex3Df> vertexMatrixVector) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(nullptr), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)
		{
		}

		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, Shader* shader) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr)
		{
		}

		~Mesh()
		{
			// delete this->MeshShader;
			// this->DeleteVAOs();
		}
	};

	class GLEObject
	{
	public:
		enum GLEObjectType
		{
			Triangle,
			Polygon
		}; 
	
		unsigned int ID; 
		
		VertexArrayObject* VAO;	//	VertexArrayObject of the mesh
		Mesh* ObjectMesh;	//	Primary mesh / 0th element of the MeshArray

		std::vector<Mesh*> MeshArray; //	Stores all the meshes created for the current instance

		void CreateObject(float*); 	// Creates a new Mesh instance with the provided vertex array's elements as its vertices, and adds it to MeshArray
		void CreateObject(std::vector<Vertex2Df>);	// Creates a new Mesh instance with the provided Vertex3D array's vertice  as its vertices
		void CreateObject(std::vector<Vertex3Df>);	// Creates a new Mesh instance with the provided Vertex3Df array's elements as its vertices
		void CreateObject(Mesh);	// Adds the provieded mesh to MeshArray	

		GLEObject();
		GLEObject(float*); 
		GLEObject(std::vector<Vertex3Df>); 
		GLEObject(std::vector<Vertex3Df>, GLEObject::GLEObjectType); 
		GLEObject(std::vector<Vertex3Df>, Shader); 
		GLEObject(std::vector<Vertex3Df>, Shader, GLenum); 
		GLEObject(float*, int, Shader); 
	};

	class Renderer
	{
	public:	
		static bool IsNull(); // Null checks all required Mesh properties
		static bool GLLoop(Window, GLEObject*);	//	Runs the OpenGL loop for the provided GLEObject.
		static bool GLLoop(Window, Mesh*);	//	 Runs the OpenGL loop for the provided Mesh.
		
		static void Render(GLEObject*);	// Renders the provided GLEObject's mesh
		static void Render(Mesh);	// Renders the provided Mesh
		static void Render(VertexArrayObject*, Shader*);	// Renders the psrovided Mesh
		static void Render(VertexArrayObject*, Shader*, Texture);	// Renders the provided Mesh, and maps its texture.
		static void Render(std::vector<VertexArrayObject*>, Shader*);	//	 Renders the provided vertex buffers.
		static void Render(std::vector<VertexArrayObject*>, Shader*, std::vector<Texture>);	//	 Renders the provided vertex buffers.
	};

	extern std::vector<GLEObject*> AllocatedGLEObjects;	//	Stores all the instances of GLEObject created durintg the execution
}

#endif
