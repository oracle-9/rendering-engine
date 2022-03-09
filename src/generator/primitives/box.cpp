#include "generator/primitives/box.hpp"

using namespace brief_int;

auto generate_box(
    u32 num_units,
    u32 grid_len,
    fmt::ostream& output_fileNaoUsado
) -> void {
    //static_cast<void>(num_units);
    //static_cast<void>(grid_len);
    //static_cast<void>(output_file);
	int num_vertices;

	num_vertices = 6 * 6 * (div * div);

	std::ofstream box;

	box.open("box.3d", std::ofstream::out | std::ios_base::app);

	box << num_vertices << endl;

	// lado esquerdo
	geraPlanoBox('x', comp, div, -1.0);

	// lado direito
	geraPlanoBox('x', comp, div, 1.0);

	// lado frente
	geraPlanoBox('z', comp, div, -1.0);

	// lado trás
	geraPlanoBox('z', comp, div, 1.0);

	// base
	geraPlanoBox('y', comp, div, -1.0);

	// topo
	geraPlanoBox('y', comp, div, 1.0);


}


auto geraPlanoBox(
	char k
    u32 comp,
    u32 div,
    u32 v,
) -> void {

	std::ofstream box;

	box.open("box.3d", std::ofstream::out | std::ios_base::app); // append or write

	//ofstream box("box.3d");

	float xNeg = -(comp / 2);
	float yNeg = -(comp / 2);
	float zNeg = -(comp / 2);
	float xPos = comp / 2;
	float yPos = comp / 2;
	float zPos = comp / 2;

	float passoX = comp / div;
	float passoY = comp / div;
	float passoZ = comp / div;

	int i, d;

	if (k == 'x') { // lado esquerdo e direito ---> depende do v
		if (v == -1.0) {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					box.print("{} {} {}\n", xPos * v, yPos, zPos);
					// p2
					box.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p3
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);



					// PONTOS DO SEGUNDO TRIANGULO
					// p3
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p2
					box.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p4
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos - passoZ);

					yPos -= passoY;
				}

				zPos -= passoZ;
			}
		}
		else {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					box.print("{} {} {}\n", xPos * v, yPos, zPos);
					// p3
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p2
					box.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);




					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					box.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p3
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p4
					box.print("{} {} {}\n", xPos * v, yPos - passoY, zPos - passoZ);

					yPos -= passoY;
				}

				zPos -= passoZ;
			}
		}
	}
	else if (k == 'y') { // topo e base ----> depende do v
		if (v == -1.0) {
			// divide por colunas
			for (int i = 1; i <= div; i++) {
				// divide por linhas
				float zNeg = -(comp / 2);
				for (int d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO ----- > p3 tem de vir antes do p2 devido à regra da mão direita
					// p1
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg);
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p3
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);


					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p4
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg + passoZ);
					// p3
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);

					zNeg += passoZ;
				}

				xNeg += passoX;
			}
		}
		else {
			// divide por colunas
			for (int i = 1; i <= div; i++) {
				// divide por linhas
				float zNeg = -(comp / 2);
				for (int d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO ----- > p3 tem de vir antes do p2 devido à regra da mão direita
					// p1
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg);
					// p3
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);


					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p3
					box.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);
					// p4
					box.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg + passoZ);

					zNeg += passoZ;
				}

				xNeg += passoX;
			}
		}
	}
	else if (k == 'z') { // frente e trás ----> depende do v
		if (v == -1.0) {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					box.print("{} {} {}\n", xNeg, yPos, zPos * v);
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p3
					box.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);



					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p4
					box.print("{} {} {}\n", xNeg + passoX, yPos - passoY, zPos * v);
					// p3
					box.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);

					yPos -= passoY;
				}

				xNeg += passoX;
			}
		}
		else {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					box.print("{} {} {}\n", xNeg, yPos, zPos * v);
					// p3
					box.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);



					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					box.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p3
					box.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);
					// p4
					box.print("{} {} {}\n", xNeg + passoX, yPos - passoY, zPos * v);

					yPos -= passoY;
				}

				xNeg += passoX;
			}
		}
	}
	else {
		cout << "Nao recebi um valor 0 no geraPlanoBox\n";
	}

	box.close();




}