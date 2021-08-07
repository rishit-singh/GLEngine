#ifndef RENDERER_H

#include "window.h"
#include "vertexarray.h"
#include "texture.h"
#include "fileio.h"
#include "tools.h"
namespace GLEngine
{
	extern GLEngine::Shader* DefaultShader;		//	Default shader. 
	extern GLEngine::Texture DefaultTexture;	//	Default texture.

	struct MVPMatrixObject	//	Stores the Model, View and Projection matrices.
	{
		glm::mat4 Model,	//	Model matrix
				View,		//	View Matrix
				Projection;	//	Projection matrix

		MVPMatrixObject(glm::mat4 model, glm::mat4 view, glm::mat4 projection) : Model(model), View(view), Projection(projection)
		{
		}
	};

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

		MVPMatrixObject* MVPMatrix;	//	Model View Projection matrix of the current mesh

		float* VertexMatrixArray;

		void Update(unsigned int);	//	Recompiles all the shaders and resets the VAO at the provided index
		
		bool AddVertexArrayObject(VertexArrayObject*);	//	Adds the provided VertexArrayObject to the current mesh.
		bool AddBufferObject(VertexBufferObject, unsigned int);	//	Adds the provided VertexBufferObject to the VertexArrayObject at the provided index.

		bool IsValid();
		
		Mesh() : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(nullptr), MatrixSize(0), MeshShader(nullptr), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MVPMatrix(nullptr)
		{
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MVPMatrix(nullptr)
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));
			
			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();
			
			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MVPMatrix(nullptr)
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));
	
			if (this->MeshTexture->IsValid())
				this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));

			this->VertexArrayObjects.back()->SetVertexAttributePointer();
			
			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MVPMatrix(nullptr)
		{			
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));

			this->VertexArrayObjects.back()->SetVertexAttributePointer();
		}

		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MVPMatrix(nullptr)
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			bool valid;

			// if (valid = this->MeshTexture->IsValid())
			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(VertexArrayObject* vertexArrayObject, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MVPMatrix(nullptr)
		{
			this->VertexArrayObjects.push_back(vertexArrayObject);	

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));	
			
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}
		
		// Mesh(VertexArrayObject vertexArrayObject, MVPMatrix mvpMatrix Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MVPMatrix(mvpMatrix)
		// {
		// 	this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			
		// 	this->VertexArrayObjects.back()->SetVertexAttributePointer();

		// 	this->MeshTexture->Bind();
		// 	this->MeshTexture->SendToShader(this->MeshShader);
		// }

		Mesh(std::vector<Vertex3Df> vertexMatrixVector) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), 
															MeshShader(new Shader(GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::VertexShader]), GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::FragmentShader]))), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(new Texture(DefaultPaths[(int)Textures][0])), MVPMatrix(nullptr)
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), GLEngine::General::GetRangeArray<unsigned int>(size)));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(std::vector<Vertex3Df> vertexMatrixVector, std::vector<unsigned int> indexArray) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), 
															MeshShader(new Shader(GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::VertexShader]), GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::FragmentShader]))), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(new Texture(DefaultPaths[(int)Textures][0])), MVPMatrix(nullptr)
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, Shader* shader, Texture* texture) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MVPMatrix(nullptr)
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), GLEngine::General::GetRangeArray<unsigned int>(vertexMatrixVector.size())));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}


		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, std::vector<unsigned int> indexArray, Shader* shader, Texture* texture) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MVPMatrix(nullptr)
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
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
			Unkown = -1,
			Triangle,
			Polygon,//	Generic polygon
			Rectangle = (int)Polygon,
			Square = (int)Rectangle,
			Cuboid,
		};

		unsigned int ID;	//	Object ID

		GLEObjectType Type;	//	Current object type		
		
		
		Mesh ObjectMesh;	//	Primary mesh / 0th element of the MeshArray

		std::vector<Mesh> MeshArray; //	Stores all the meshes created for the current instance

		bool IsValid();	// Checks the object validity

		void CreateObject(Mesh);	// Adds the provided mesh to MeshArray	
		void CreateObject(Mesh, GLEObjectType);	// Adds the provided mesh of the provided type to MeshArray	
		void CreateObject(std::vector<Vertex2Df>);	// Creates a new Mesh instance with the provided Vertex3D array's vertice  as its vertices
		void CreateObject(std::vector<Vertex3Df>);	// Creates a new Mesh instance with the provided Vertex3Df array's elements as its vertices
		void CreateObject(std::vector<Vertex3Df>, GLEObjectType);	//	Creates an object type as specified.
		void CreateObject(Point3Df, Point3Df, GLEObjectType);	//	Creates an object type as specified.
		void CreateObject(Point3Df, Point3Df, Texture*, GLEObjectType);	//	Creates an object type as specified with a specified texture
		
		GLEObject();
		GLEObject(float*);
		GLEObject(Mesh);
		GLEObject(Mesh, GLEObjectType);
		GLEObject(std::vector<Vertex3Df>); 
		GLEObject(std::vector<Vertex3Df>, GLEObjectType); 
		GLEObject(std::vector<Vertex3Df>, GLEObjectType, Shader*);
		GLEObject(float*, int, Shader*); 
	
	protected:
		static VertexBufferObject (*ObjectCreationFunctions[2])(Point3Df, Point3Df);
	};

	class ShapeBufferGeneration
	{
	public:
		static inline std::vector<std::vector<unsigned int>> GenericIndexBuffers = {		//	Predefined index buffers for generic shapes
			{
				0, 1, 2		//	Triangle
			},

			{
				0, 1, 2,	//	Polygon/Rectangle/Square
				2, 3, 0
			},

			{
				0, 1, 2,	//	Cuboid
				2, 3, 0,

				4, 5, 6,
				6, 7, 4,

				8, 9, 10,
				10, 11, 8,

				12, 13, 14,
				14, 15, 12,

				16, 17, 18,
				18, 19, 16,

				20, 21, 22,
				22, 23, 20	
			}	
		};

		static VertexBufferObject GenerateTriangle(std::vector<Vertex3Df>);	//	Generates a vertex buffer object for a triangle with provided vertices\

		static VertexBufferObject GeneratePolygon(std::vector<Point3Df>, std::vector<Point3Df>);	//	Generates a vertex buffer object for a generic polygon with provided vertices
	
		static VertexBufferObject GenerateRectangle(GLEngine::Point3Df, GLEngine::Point3Df);	//	Generates a vertex buffer object for a rectangle with provided vertices at the provided vertices
		static std::vector<float> GenerateRectangleVertices(GLEngine::Point3Df, GLEngine::Point3Df);	//	Generates an array of vertices for a rectangle with provided vertices at the provided location
		static std::vector<Vertex3Df> GenerateRectangleVertexObjects(GLEngine::Point3Df, GLEngine::Point3Df);	//	Generates an array of Vertex3Df for a rectangle with provided vertices at the provided location
		
		static VertexBufferObject GenerateCuboid(GLEngine::Point3Df, GLEngine::Point3Df);		//	Generates a vertex buffer object for a cuboid with provided vertices at the provided location
		static std::vector<float> GenerateCuboidVertices(GLEngine::Point3Df, GLEngine::Point3Df);		//	Generates a vertex buffer object for a cuboid with provided vertices at the provided location
		static std::vector<Vertex3Df> GenerateCuboidVertexObjects(GLEngine::Point3Df, GLEngine::Point3Df);		//	Generates a vertex buffer object for a cuboid with provided vertices at the provided location
	};

	class Renderer
	{
	public:	
		static bool IsNull(); // Null checks all required Mesh properties
		static bool GLLoop(Window, GLEObject*);	//	Runs the OpenGL loop for the provided GLEObject.
		static bool GLLoop(Window, Mesh*);	//	 Runs the OpenGL loop for the provided Mesh.
		
		static void Render(GLEObject);	// Renders the provided GLEObject's mesh
		static void Render(Mesh);	// Renders the provided Mesh
		static void Render(VertexArrayObject*, Shader*);	// Renders the psrovided Mesh
		static void Render(VertexArrayObject*, Shader*, Texture);	// Renders the provided Mesh, and maps its texture.
		static void Render(std::vector<VertexArrayObject*>, Shader*);	//	 Renders the provided vertex buffers.
		static void Render(std::vector<VertexArrayObject*>, Shader*, std::vector<Texture>);	//	 Renders the provided vertex buffers.
	};

	extern std::vector<GLEObject*> AllocatedGLEObjects;	//	Stores all the instances of GLEObject created durintg the execution
}

#endif
