#include "generator/primitives/sphere.hpp"

using namespace brief_int;

auto generate_sphere(
    u32 radius,
    u32 num_slices,
    u32 num_stacks,
    fmt::ostream& output_file
) -> void {

    float rotationSlice = (float) 2 * M_PI / num_slices;
	float rotationStack = (float) M_PI / num_stacks;

	// x= r * cos(beta)  * sin(alpha);
	// y= r * sin(beta);
	// z= r * cos(beta)  * cos(alpha);

    int num_vertices = 2*3*num_slices + 2*3*(num_stacks-2)*num_slices; // (faces 1º e 2º stack) + (faces das restantes stacks)

    output_file.print("{}\n",num_vertices);

	for (int i = 0; i < (int) num_stacks; i++) {

		float stackAngle = (float) (M_PI / 2) - i * rotationStack; // beta
		float stackAngle2 = (float) (M_PI / 2) - (i+1) * rotationStack; // beta de baixo

		for (int j = 0; j < (int) num_slices; j++) {

			float sliceAngle = j * rotationSlice; // alpha
			float sliceAngle2 = (j+1) * rotationSlice; // alpha do lado

			if (i < num_stacks-1) {
            	output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
            	output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle));
            	output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
			}
			else{
				output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle2), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
			}

			if (i > 0 && i < (int) num_stacks-1) {
                output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle2), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
			}
		}
	}
}
