#include "generator/primitives/sphere.hpp"

using namespace brief_int;

auto generate_sphere(
    u32 radius,
    u32 num_slices,
    u32 num_stacks,
    fmt::ostream& output_file
) -> void {
    static_cast<void>(radius);
    static_cast<void>(num_slices);
    static_cast<void>(num_stacks);
    static_cast<void>(output_file);

    double rotationSlice = 2 * M_PI / num_slices;
	double rotationStack = M_PI / num_stacks;

	// x= r * cos(beta)  * sin(alpha);
	// y= r * sin(beta);
	// z= r * cos(beta)  * cos(alpha);

    int num_vertices = 2*num_slices+(num_stacks-2)*2*num_slices;

    output_file.print("{}\n",num_vertices);

	for (int i = 0; i < num_stacks; i++) {

		float stackAngle = (M_PI / 2) - i * rotationStack; // beta
		float stackAngle2 = (M_PI / 2) - (i+1) * rotationStack; // beta de baixo

		for (int j = 0; j <= num_slices; j++) {

			float sliceAngle = j * rotationSlice; // alpha
			float sliceAngle2 = (j+1) * rotationSlice; // alpha do lado

            output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
            output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle));
            output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));

			if (i > 0 && i < num_stacks) {
                output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle2), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
			}
		}
	}
}
