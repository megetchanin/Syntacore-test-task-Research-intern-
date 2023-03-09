#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

// Here is the code of the task
// code in comment below is a version for multithreading where we execute xor operation with multiple threads
// <Windows.h> is not a standart library


/*struct args_for_func {
	std::vector<unsigned long long> vector_field;
	unsigned long long length_of_str;
	unsigned long long* res_array;
	unsigned long long threads;
	unsigned long long start_index;
};

DWORD WINAPI Xor_threaded_func(LPVOID args) {
	args_for_func* arg = (args_for_func*)args;

	std::vector<unsigned long long> input_values = arg->vector_field;
	unsigned long long length_of_str = arg->length_of_str;
	unsigned long long* res_array = arg->res_array;
	unsigned long long threads = arg->threads;
	printf("input_values_size: %d\n", input_values.size());
	for (arg->start_index; arg->start_index < (1ULL << input_values.size()); arg->start_index += threads) {
		unsigned long long bool_choice;
		unsigned long long out_value = 0;
		for (unsigned long long this_value = 0; this_value < input_values.size(); this_value++) {
			if (arg->start_index & (1ULL << this_value)) {
				bool_choice = 1;
			}
			else { bool_choice = 0; }
			out_value = out_value ^ (bool_choice * input_values[this_value]);
		}
		unsigned long long weight_of_value = 0;
		for (unsigned long long bit = 0; bit < length_of_str; bit++) {
			if (out_value & (1ULL << bit)) {
				weight_of_value++;
			}
		}
		res_array[weight_of_value]++;
	}
	return 0;
};*/

//unsigned long long* res_array_1 = new unsigned long long[length_of_str + 1]{};
//unsigned long long* res_array_2 = new unsigned long long[length_of_str + 1]{};
//unsigned long long* res_array = new unsigned long long[length_of_str + 1]{};
//xor_sum(input_int_values, length_of_str, res_array);

//args_for_func first_struct{ input_int_values, length_of_str, res_array_1, 2, 0 };
//args_for_func second_struct{ input_int_values, length_of_str, res_array_2, 2, 1 };
//clock_t tStart = clock();
//HANDLE hThread_1 = CreateThread(NULL, 0, Xor_threaded_func, &first_struct, 0, NULL);
//HANDLE hThread_2 = CreateThread(NULL, 0, Xor_threaded_func, &second_struct, 0, NULL);

//HANDLE arr[2] = { hThread_1, hThread_2 };
//WaitForMultipleObjects(2, arr, 1, INFINITE);
//printf("Time taken: %.5f ms\n", (double)(clock() - tStart) / (CLOCKS_PER_SEC / 1000));
//CloseHandle(hThread_1);
//CloseHandle(hThread_2);


// Function for converting strings from input file to decimal values
std::vector<int> string_to_int(std::vector<std::string> input_vector, size_t &length_of_str) {
	std::vector<int> out_int_vector;
	length_of_str = input_vector[0].length();
	printf("length_of_str: %d\n", length_of_str);
	for (int number_of_str = 0; number_of_str < input_vector.size(); number_of_str++) {
		//input_vector[number_of_str].pop_back();
		int value = 0;
		for (int bit = 0; bit < length_of_str; bit++) {
			value = value << 1;
			if (input_vector[number_of_str][bit] == '1'){
				++value;
			}
		}
		out_int_vector.push_back(value);
	}
	return out_int_vector;
};
// function of xor
void xor_sum(std::vector<int> input_values, int length_of_str, int* res_array) {
	// this cycle is running through possible values of beta that are specified by number of input vectors
	for (unsigned long long this_beta = 0; this_beta < pow(2, input_values.size()); this_beta++) {
		int bool_choice;
		int out_value = 0;
		// this cycle is running through all vectors from input file. Length of any beta in binary representation is equal to number
		// of input vectors, so on each step of this cycle we take one bit from beta and decide if we need to add this value or not. 
		for (int this_value = 0; this_value < input_values.size(); this_value++) {
			if (this_beta & (1ULL << this_value)) {
				bool_choice = 1;
			}
			else { bool_choice = 0; }
			out_value = out_value ^ (bool_choice * input_values[this_value]);}
		// here we count weight of value by finding 'ones' in binary representation of "xored" value
		int weight_of_value = 0;
		for (int bit = 0; bit < length_of_str; bit++) {
			if (out_value & (1 << bit)) {
				weight_of_value++;
			} 
		}
		res_array[weight_of_value]++;
	}
}

int main() {
	clock_t tStart = clock();
	// creating descriptor of input file and opening it
	std::ifstream input_file;
	input_file.open("test_task_input.txt");
	// creating vector of strings in "binary representation" (string consists of 'ones' and 'zeros')
	std::vector<std::string> input_binary_strings;
	std::string line_from_file;
	std::string prev_line;
	// reading strings from file
	if (input_file.is_open()) {
		while (!input_file.eof()) {
			input_file >> line_from_file;
			if (line_from_file == prev_line) {
				break;
			}
			input_binary_strings.push_back(line_from_file);
			prev_line = line_from_file;
		}
	}
	else {
		printf("File wasn't opened");
		return 1;
	}
	input_file.close();
	size_t length_of_str;
	// getting decimal representation of input strings
	std::vector<int> input_int_values = string_to_int(input_binary_strings, length_of_str);
	int* res_array = new int[length_of_str+1];
	std::fill(res_array, res_array+length_of_str+1, 0);
	//memset(res_array, 0, (sizeof(int))*length_of_str+1);
	// operation of xor
	xor_sum(input_int_values, length_of_str, res_array);
	// opening output file and writing values sorted by weight
	std::ofstream output_file;
	output_file.open("test_task_output.txt", std::ios::out);
	if (output_file.is_open()) {
		for (int i = 0; i < (length_of_str + 1); i++) {
			output_file << i << "\t" << res_array[i] << "\n";
		}
	}
	else {
		printf("File wasn't opened\n");
		return 1;
	}
	output_file.close();
	
	delete[] res_array;
	printf("Time taken: %.5f ms\n", (double)(clock() - tStart) / (CLOCKS_PER_SEC/1000));
}

