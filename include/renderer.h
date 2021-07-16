#ifndef RENDERER_H

#include "vertexarray.h"
#include "window.h"

namespace GLEngine
{

	class Mesh 	//	3D/2D meshes
	{
	public:
		int MatrixSize;	//	VertexMatrix array length

		VertexArrayObject* VAO;	// Vertex Array Object 
		
		std::vector<Vertex3Df> VertexMatrixVector;	// Stores the triangle vertex matrix of a maesh

		Shader MeshShader;	// Shader object of the current mesh

		float* VertexMatrixArray;

		void SetVAO();	//	Sets the Vertex Array Object
		bool Enable();	//	Sets up the mesh by enabling the vertex array object and shaders	
		bool Update();	//	Recompiles all the shaders and resets the VAO

		Mesh() : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(new float[1000]), MatrixSize(0), MeshShader(Shader()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL)) 
		{
		}
		
		Mesh(Vertex3Df* vertexMatrix, int size) : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(General::PointArrayToFloatArray(vertexMatrix, size)), MeshShader(Shader()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL))
		{
		}

		Mesh(float* vertexMatrix, int size, Shader shader) : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(vertexMatrix), MeshShader(shader),VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL))		{
		}

		Mesh(std::vector<Vertex3Df> vertexMatrixVector) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(Shader()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL))
		{
		}

		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, Shader shader) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL))
		{
			// if (vertexMatrixVector.size() > 0) 
				// this->SetVAO();
		}

		~Mesh()
		{
			delete &this->MeshShader;
			delete &this->VAO;
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
		static void Render(Mesh*);	// Renders the provided Mesh
		static void Render(VertexArrayObject*, Shader*);	// Renders the psrovided Mesh
		static void Render(std::vector<VertexArrayObject*>, Shader*);	//	 Renders the provided vertex buffers.
	};

	extern std::vector<GLEObject*> AllocatedGLEObjects;	//	Stores all the instances of GLEObject created durintg the execution
}

#endif
