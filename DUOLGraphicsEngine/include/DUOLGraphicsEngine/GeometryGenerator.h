#pragma once
#include <vector>
#include "DUOLMath/DUOLMath.h"
#include "ResourceManager/Resource/Vertex.h"

//thanks to Muscle Graphics!:D
namespace DUOLGraphicsEngine
{
	class GeometryGenerator
	{
	public:
		static void CreateBox(float width, float height, float depth, std::vector<StaticMeshVertex>& vertices, std::vector<unsigned int>& indices)
		{
			StaticMeshVertex v[24];

			float w2 = 0.5f * width;
			float h2 = 0.5f * height;
			float d2 = 0.5f * depth;

			// Fill in the front face vertex data.
			v[0] = StaticMeshVertex({ -w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
			v[1] = StaticMeshVertex({ -w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
			v[2] = StaticMeshVertex({ +w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });
			v[3] = StaticMeshVertex({ +w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });

			// Fill in the back face vertex data.
			v[4] = StaticMeshVertex({ -w2, -h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });
			v[5] = StaticMeshVertex({ +w2, -h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
			v[6] = StaticMeshVertex({ +w2, +h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
			v[7] = StaticMeshVertex({ -w2, +h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });

			// Fill in the top face vertex data.
			v[8] = StaticMeshVertex({ -w2, +h2, -d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
			v[9] = StaticMeshVertex({ -w2, +h2, +d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
			v[10] = StaticMeshVertex({ +w2, +h2, +d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });
			v[11] = StaticMeshVertex({ +w2, +h2, -d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });

			// Fill in the bottom face vertex data.
			v[12] = StaticMeshVertex({ -w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });
			v[13] = StaticMeshVertex({ +w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
			v[14] = StaticMeshVertex({ +w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
			v[15] = StaticMeshVertex({ -w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });

			// Fill in the left face vertex data.
			v[16] = StaticMeshVertex({ -w2, -h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f });
			v[17] = StaticMeshVertex({ -w2, +h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f });
			v[18] = StaticMeshVertex({ -w2, +h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f });
			v[19] = StaticMeshVertex({ -w2, -h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f });

			// Fill in the right face vertex data.
			v[20] = StaticMeshVertex({ +w2, -h2, -d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f });
			v[21] = StaticMeshVertex({ +w2, +h2, -d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f });
			v[22] = StaticMeshVertex({ +w2, +h2, +d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f });
			v[23] = StaticMeshVertex({ +w2, -h2, +d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f });

			vertices.assign(&v[0], &v[24]);

			unsigned i[36];

			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			indices.assign(&i[0], &i[36]);
		}

		static void CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, std::vector<StaticMeshVertex>& vertices, std::vector<unsigned int>& indices)
		{
			//
			// Compute the vertices stating at the top pole and moving down the stacks.
			//

			// Poles: note that there will be texture coordinate distortion as there is
			// not a unique point on the texture map to assign to the pole when mapping
			// a rectangular texture onto a sphere.
			StaticMeshVertex topVertex({ 0.0f, +radius, 0.0f }, { 0.0f, +1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
			StaticMeshVertex bottomVertex({ 0.0f, -radius, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });

			vertices.push_back(topVertex);

			float phiStep = DUOLMath::PI / stackCount;
			float thetaStep = 2.0f * DUOLMath::PI / sliceCount;

			// Compute vertices for each stack ring (do not count the poles as rings).
			for (unsigned int i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i * phiStep;

				// Vertices of ring.
				for (unsigned int j = 0; j <= sliceCount; ++j)
				{
					float theta = j * thetaStep;

					StaticMeshVertex v;

					// spherical to cartesian
					v._pos.x = radius * sinf(phi) * cosf(theta);
					v._pos.y = radius * cosf(phi);
					v._pos.z = radius * sinf(phi) * sinf(theta);

					// Partial derivative of P with respect to theta
					v._tangent.x = -radius * sinf(phi) * sinf(theta);
					v._tangent.y = 0.0f;
					v._tangent.z = +radius * sinf(phi) * cosf(theta);

					v._tangent.Normalize();

					v._normal = v._pos;
					v._normal.Normalize();

					v._texcoord.x = theta / (DUOLMath::PI*2);
					v._texcoord.y = phi / DUOLMath::PI;

					vertices.push_back(v);
				}
			}

			vertices.push_back(bottomVertex);

			//
			// Compute indices for top stack.  The top stack was written first to the vertex buffer
			// and connects the top pole to the first ring.
			//

			for (unsigned i = 1; i <= sliceCount; ++i)
			{
				indices.push_back(0);
				indices.push_back(i + 1);
				indices.push_back(i);
			}

			//
			// Compute indices for inner stacks (not connected to poles).
			//

			// Offset the indices to the index of the first vertex in the first ring.
			// This is just skipping the top pole vertex.
			unsigned baseIndex = 1;
			unsigned ringVertexCount = sliceCount + 1;

			for (unsigned i = 0; i < stackCount - 2; ++i)
			{
				for (unsigned j = 0; j < sliceCount; ++j)
				{
					indices.push_back(baseIndex + i * ringVertexCount + j);
					indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

					indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
					indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
				}
			}

			//
			// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
			// and connects the bottom pole to the bottom ring.
			//

			// South pole vertex was added last.
			unsigned southPoleIndex = (unsigned)vertices.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (unsigned i = 0; i < sliceCount; ++i)
			{
				indices.push_back(southPoleIndex);
				indices.push_back(baseIndex + i);
				indices.push_back(baseIndex + i + 1);
			}
		}
	};
}
