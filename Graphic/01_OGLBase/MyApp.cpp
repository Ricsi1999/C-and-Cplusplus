#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitTriangle()
{
	std::vector<Vertex>vertices;
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });

	std::vector<int> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	m_TriangleVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_TriangleIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_TriangleVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_TriangleVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_TriangleVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_TriangleVertexBuffer },
		},
		m_TriangleIndices
	);
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].n *= -1.0f;
	}

	std::vector<int> indices(24);
	int index = 0;
	for (int i = 0; i < 4 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 2] = i + 1;
		indices[index + 1] = i + 2;

		indices[index + 3] = i + 1;
		indices[index + 5] = i + 3;
		indices[index + 4] = i + 2;
		index += 6;
	}

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_CubeVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következõ BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_CubeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülsõ lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("assets/xpos.png", true, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("assets/xneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("assets/ypos.png", true, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/yneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/zpos.png", true, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// a GL_TEXTURE_MAG_FILTER-t és a GL_TEXTURE_MIN_FILTER-t beállítja az AttachFromFile
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= 3.1415f;
	float r = 1;

	return glm::vec3(
		r * sin(v) * cos(u),
		r * cos(v),
		r * sin(v) * sin(u));
}

glm::vec3 CMyApp::GetNorm(float u, float v)
{
	// Képlettel
	u *= float(2 * M_PI);
	v *= float(M_PI);
	return glm::vec3(sin(v) * cos(u), cos(v), sin(v) * sin(u));

	// Numerikusan (nem kell ismerni a képletet, elég a pozícióét)
	/*
	glm::vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	glm::vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return glm::normalize(glm::cross(du, dv));*/
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}

void CMyApp::InitSphere()
{
	// NxM darab négyszöggel közelítjük a parametrikus felületünket => (N+1)x(M+1) pontban kell kiértékelni
	Vertex vert[(N + 1) * (M + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = GetSpherePos(u, v);
			vert[i + j * (N + 1)].n = GetNorm(u, v);
			vert[i + j * (N + 1)].t = GetTex(u, v);
		}

	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
	int indices[3 * 2 * (N) * (M)];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}



	m_SphereVertexBuffer.BufferData(vert);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SphereIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_SphereVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
		},
		m_SphereIndices
	);
}

void CMyApp::InitSurface()
{
	std::vector<Vertex>vertices;
	std::vector<int> indices;

	for (int x = 0; x <= 20; x++) { //x tengely
		for (int z = 0; z <= 20; z++) { //z tengely
			Vertex v;

			v.p = glm::vec3( //pozíció
				x - 10.0f,
				sin(x),
				z - 10.0f
			);
			v.n = glm::vec3(0.0f, 1.0f, 0.0f); //normálvektor
			v.t = glm::vec2(x, z) / 20.0f; //textúra koordináta

			vertices.push_back(v);
		}
	}

	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			int topLeft = x * 21 + z;
			int topRight = topLeft + 21; // (x + 1) * 21 + z
			int bottomLeft = topLeft + 1; // (x * 21) + z + 1
			int bottomRight = topRight + 1; // (x + 1) * 21 + (z + 1)

			indices.push_back(bottomRight);
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);

			indices.push_back(bottomRight);
			indices.push_back(topRight);
			indices.push_back(topLeft);

			// Számoljuk ki a normálvektorokat is

			glm::vec3 a, b, c, n;

			//Első háromszög
			a = vertices[bottomRight].p;
			b = vertices[topLeft].p;
			b = vertices[bottomLeft].p;

			n = glm::normalize(glm::cross(c - b, a - b));

			vertices[bottomRight].n = n;
			vertices[topLeft].n = n;
			vertices[topLeft].n = n;

			//Második háromszög

			a = vertices[bottomRight].p;
			b = vertices[topRight].p;
			b = vertices[bottomLeft].p;

			n = glm::normalize(glm::cross(c - b, a - b));

			vertices[bottomRight].n = n;
			vertices[topRight].n = n;
			vertices[topLeft].n = n;
		}
	}

	m_SurfaceVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SurfaceIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_SurfaceVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_SurfaceVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SurfaceVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SurfaceVertexBuffer },
		},
		m_SurfaceIndices
	);
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);


	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitCube();
	InitSkyBox();
	InitSurface();
	InitSphere();
	//InitTriangle();

	// egyéb textúrák betöltése
	m_woodTexture.FromFile("assets/wood.jpg");
	m_barkTexture.FromFile("assets/bark.jpg");
	m_leaveTexture.FromFile("assets/leaves.jpg");

	m_forestTexture.FromFile("assets/forest.jpg");
	m_grassTexture.FromFile("assets/grass.jpg");

	// mesh betoltese
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/henger.obj"));
	m_mesh->initBuffers();
	
	// kamera
	m_camera.SetProj(60.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	int numberOfTrees = rand() % 51 + 50;

	for (int i = 0; i < numberOfTrees; i++) {

		int x = rand() % 19 + 1;
		int z = rand() % 19 + 1;

		m_trees.push_back(
			glm::vec3(
				x - 10.0f,
				sin(x),
				z - 10.0f
			)
		);
	}

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat

	glm::mat4 viewProj = m_camera.GetViewProj();

	//Fatörzs
	glm::mat4 treeWorld = glm::mat4(1.0f);
	m_program.Use();
	m_program.SetTexture("texImage", 0, m_barkTexture);

	for (int i = 0; i < m_trees.size(); i++) {
		treeWorld = glm::translate(m_trees[i] - glm::vec3(0.0f, 1.0f, 0.0f));
		m_program.SetUniform("MVP", viewProj * treeWorld);
		m_program.SetUniform("world", treeWorld);
		m_program.SetUniform("worldIT", glm::inverse(glm::transpose(treeWorld)));
		m_mesh->draw();
	}

	//glDisable(GL_CULL_FACE);

	//Háromszög
	//m_TriangleVao.Bind();
	//m_program.SetTexture("texImage", 0, m_woodTexture);
	//for (int i = 0; i < N; i++) {
	//	glm::mat4 triangleWorld = glm::rotate<float>(i + 1, glm::vec3(1.0f, 0.0f, 0.0f));
	//	m_program.SetUniform("MVP", viewProj * triangleWorld);
	//	m_program.SetUniform("world", triangleWorld);
	//	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(triangleWorld)));
	//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	//}



	//glEnable(GL_CULL_FACE);

	//Surface
	m_SurfaceVao.Bind();
	m_program.SetTexture("texImage", 0, m_grassTexture);
	glm::mat4 surfaceWorld = glm::mat4(1.0f);
	surfaceWorld = glm::translate(glm::vec3(0.0f, -3.0f, 0.0f));
	m_program.SetUniform("MVP", viewProj * surfaceWorld);
	m_program.SetUniform("world", surfaceWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(surfaceWorld)));
	glDrawElements(GL_TRIANGLES, 20 * 20 * 6, GL_UNSIGNED_INT, nullptr);



	// Lombok
	m_SphereVao.Bind();
	m_program.SetTexture("texImage", 0, m_leaveTexture);
	glm::mat4 leaveWorld = glm::mat4(1.0f);
	for (int i = 0; i < m_trees.size(); i++) {
		leaveWorld = glm::translate(m_trees[i] + glm::vec3(0.0f, 2.0f, 0.0f));
		m_program.SetUniform("MVP", viewProj * leaveWorld);
		m_program.SetUniform("world", leaveWorld);
		m_program.SetUniform("worldIT", glm::inverse(glm::transpose(leaveWorld)));
		glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, nullptr);
	}


	m_SphereVao.Bind();
	m_program.SetTexture("texImage", 0, m_woodTexture);
	glm::mat4 circleWorld = glm::scale(glm::vec3(1.0f, 1.0f, 0.0001f));
	m_program.SetUniform("MVP", viewProj * circleWorld);
	m_program.SetUniform("world", circleWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(circleWorld)));
	glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, nullptr);



	// kockák
	//m_program.Use(); nem hívjuk meg újra, hisz ugyanazt a shadert használják
	m_CubeVao.Bind();
	m_program.SetTexture("texImage", 0, m_forestTexture);
	glm::mat4 cubeWorld = glm::scale(glm::vec3(20.0f, 8.0f, 20.0f));

	m_program.SetUniform("MVP", viewProj * cubeWorld);
	m_program.SetUniform("world", cubeWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cubeWorld)));
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);

	m_program.Unuse();

	// skybox
	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);


	// 1. feladat: készíts egy vertex shader-fragment shader párt, ami tárolt geometria _nélkül_ kirajzol egy tetszõleges pozícióba egy XYZ tengely-hármast,
	//			   ahol az X piros, az Y zöld a Z pedig kék!

	//ImGui Testwindow
	ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
