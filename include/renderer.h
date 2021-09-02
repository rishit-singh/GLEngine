#ifndef RENDERER_H

#include <math.h>
#include "window.h"
#include "vertexarray.h"
#include "texture.h"
#include "blending.h"
#include "fileio.h"
#include "tools.h"
#include "transform.h"
#include "worldspace.h"


#define DOUBLEPI M_PI * 2

namespace GLEngine
{
	extern GLEngine::Shader* DefaultShader;		//	Default shader. 
	extern GLEngine::Texture DefaultTexture;	//	Default texture.

	extern GLEngine::Blender DefaultBlender;	//	Default blender

	extern GLEngine::WorldSpace DefaultWorldSpace;

	class Mesh	//	3D/2D Mesh
	{
	private:
		Shader m_MeshShader;

		Texture m_MeshTexture;

		void DeleteVAOs();	//	Deletes all the allocated VAOs in the current mesh instance.

	public:
		int MatrixSize;	//	VertexMatrix array length

		std::vector<VertexArrayObject*> VertexArrayObjects;	// Stores all the vertex array objects 
		
		std::vector<Vertex3Df> VertexMatrixVector;	// Stores the triangle vertex matrix of a maesh

		std::vector<Transform> MeshTransforms;	//	Stores all the transforms of the currnet mesh

		Shader* MeshShader;	// Shader* object of the current mesh
	
		Texture* MeshTexture;	//	Texture to be mapped on the mesh objects.

		Transform MeshTransform;	//	The head/main transform of the current mesh instance.
		
		float* VertexMatrixArray;

		void Update(unsigned int);	//	Recompiles all the shaders and resets the VAO at the provided index	
		
		void AddClone();	//	Creates a clone transform of the current mesh
		
		void SetMVP();	//	Sets the model, view and projection for the current Mesh shader 
		void SetMVP(WorldSpace);	//	Sets the model, view and projection for the current Mesh shader using the provided world space 
		
		bool IsValid();
		
		bool AddVertexArrayObject(VertexArrayObject*);	//	Adds the provided VertexArrayObject to the current mesh.
		bool AddBufferObject(VertexBufferObject, unsigned int);	//	Adds the provided VertexBufferObject to the VertexArrayObject at the provided index.
		
		
		Mesh() : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(nullptr), MatrixSize(0), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();
			
			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(float* vertexBufferArray, unsigned int vertexBufferSize, unsigned int* indexBufferArray, unsigned int indexBufferSize, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), m_MeshTexture(*texture), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, vertexBufferSize, indexBufferArray, indexBufferSize));
	
			if (this->MeshTexture->IsValid())
				this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));

			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	 For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			delete vertexBufferArray, indexBufferArray;
		}
		
		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(nullptr), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{			
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();
		}

		Mesh(std::vector<float> vertexBufferArray, std::vector<unsigned int> indexArray, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			this->VertexArrayObjects.push_back(new VertexArrayObject(vertexBufferArray, indexArray));

			bool valid;

			// if (valid = this->MeshTexture->IsValid())
			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

 			this->MeshTexture = &this->m_MeshTexture;

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(VertexArrayObject* vertexArrayObject, Shader* shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			this->VertexArrayObjects.push_back(vertexArrayObject);	

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->MeshTexture = &this->m_MeshTexture;

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(VertexArrayObject* vertexArrayObject, Shader shader, Texture* texture) : VertexMatrixVector(std::vector<Vertex3Df>()), VertexArrayObjects(std::vector<VertexArrayObject*>()), MeshTexture(texture), m_MeshShader(shader), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			this->VertexArrayObjects.push_back(vertexArrayObject);	

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));	
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();


			this->MeshShader = &this->m_MeshShader;

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
															MeshShader(new Shader(GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::VertexShader]), GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::FragmentShader]))), VertexArrayObjects(std::vector<VertexArrayObject*>()), m_MeshTexture(Texture(DefaultPaths[(int)Textures][0])),
															MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), GLEngine::General::GetRangeArray<unsigned int>(size)));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture = &this->m_MeshTexture;

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(std::vector<Vertex3Df> vertexMatrixVector, std::vector<unsigned int> indexArray) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), 
															MeshShader(new Shader(GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::VertexShader]), GLEngine::FileIO::Read(DefaultPaths[(int)Shaders][(int)Shader::FragmentShader]))), VertexArrayObjects(std::vector<VertexArrayObject*>()), m_MeshTexture(Texture(DefaultPaths[(int)Textures][0])),
															MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture = &this->m_MeshTexture;

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}

		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, Shader* shader, Texture texture) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), m_MeshTexture(texture), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), GLEngine::General::GetRangeArray<unsigned int>(vertexMatrixVector.size())));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture = &this->m_MeshTexture;

			this->MeshTexture->Bind();
			this->MeshTexture->SendToShader(this->MeshShader);
		}


		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, std::vector<unsigned int> indexArray, Shader* shader, Texture texture) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader), VertexArrayObjects(std::vector<VertexArrayObject*>()), m_MeshTexture(texture), MeshTransform(Transform({ 0.1f, 0.0f, -2.0f }))
		{
			int size = vertexMatrixVector.size() * 3;	//	size temp

			this->VertexArrayObjects.push_back(new VertexArrayObject(GLEngine::General::VertexVectorToFloatVector(vertexMatrixVector), indexArray));

			this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
			this->VertexArrayObjects.back()->SetVertexAttributePointer();

			// this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.back()->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));	//	For normals
			// this->VertexArrayObjects.back()->SetVertexAttributePointer();

			this->MeshTexture = &this->m_MeshTexture;

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
		
		void Delete();	//	Deletes all the heap allocated objects of the current instance
		
		GLEObject();
		GLEObject(float*);
		GLEObject(Mesh);
		GLEObject(Mesh, GLEObjectType);
		GLEObject(std::vector<GLEngine::VertexBufferObject>, std::vector<GLEngine::Texture*>);
		GLEObject(std::vector<Vertex3Df>); 
		GLEObject(std::vector<Vertex3Df>, GLEObjectType); 
		GLEObject(std::vector<Vertex3Df>, GLEObjectType, Shader*);
		GLEObject(float*, int, Shader*); 
	
		~GLEObject()
		{
		}

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
		static std::vector<VertexBufferObject> GenerateCuboidFaces(GLEngine::Point3Df, GLEngine::Point3Df);		//	Generates a vertex buffer objects of rectangles making up a cuboid.
		// static std::vector<VertexBufferObject> GenerateCuboidFaces(GLEngine::Point3Df, GLEngine::Point3Df);		//	Generates a vertex buffer objects of rectangles making up a cuboid with a specific texture for each rectangle.
	};

	class Renderer
	{
	public:	
		static bool IsNull(); // Null checks all required Mesh properties
		static bool GLLoop(Window, GLEObject*);	//	Runs the OpenGL loop for the provided GLEObject.
		static bool GLLoop(Window, Mesh*);	//	 Runs the OpenGL loop for the provided Mesh.
		
		static void Render(GLEObject);	// Renders the provided GLEObject's mesh
		static void Render(GLEObject, bool);	// Renders the provided GLEObject's mesh, indexed or otherwise	
		static void Render(Mesh);	// Renders the provided Mesh
		static void Render(VertexArrayObject*, Shader*);	// Renders the psrovided Mesh
		static void Render(VertexArrayObject*, Shader*, Texture*);	// Renders the provided Mesh, and maps its texture.
		static void Render(std::vector<VertexArrayObject*>, Shader*);	//	 Renders the provided vertex buffers.
		static void Render(std::vector<VertexArrayObject*>, Shader*, std::vector<Texture*>);	//	 Renders the provided vertex buffers.

		static void RenderNonIndexed(GLEObject);	//	Renders a non-indexed buffer.
	};

	extern std::vector<GLEObject*> AllocatedGLEObjects;	//	Stores all the instances of GLEObject created durintg the execution
}

#endif
