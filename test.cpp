#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "nonull.h"

void print_msg(const std::string &msg)
{
	std::cerr << msg << '\n';
}

const std::size_t N = 10;

void print_vec(std::unique_ptr<std::vector<float>> vec) { for (std::size_t i = 0; i < vec->size(); ++i) std::cerr << (*vec)[i] << ", "; std::cerr << '\n'; }
void print_vec(std::shared_ptr<std::vector<float>> vec) { for (std::size_t i = 0; i < vec->size(); ++i) std::cerr << (*vec)[i] << ", "; std::cerr << '\n'; }
void print_vec(std::vector<float> *vec) { for (std::size_t i = 0; i < vec->size(); ++i) std::cerr << (*vec)[i] << ", "; std::cerr << '\n'; }

void print_vec_raw1(float *vec) { for (std::size_t i = 0; i < N; ++i) std::cerr << vec[i] << ", "; std::cerr << '\n'; }
void print_vec_raw2(nonull_ptr<float> vec) { for (std::size_t i = 0; i < N; ++i) std::cerr << vec[i] << ", "; std::cerr << '\n'; }

int main()
{
	nonull_ptr<int> ip = new int;
	ip = new int;

	nonull_ptr<const int> cip = new int(77);

	int *_ip = nullptr;
	//ip = nullptr;
	//ip = _ip;

	nonull_unique_ptr<int> iup = new int;

	nonull_unique_ptr<const int> ciup = new int(77);

	*ip = 6;
	std::cerr << *ip << '\n';
	std::cerr << *cip << '\n';

	std::cerr << '\n';

	*iup = 6;
	std::cerr << *iup << '\n';
	std::cerr << *ciup << '\n';

	std::cerr << '\n';

	nonull_ptr<float> rvec = new float[N];
	nonull_ptr<std::vector<float>> pvec = new std::vector<float>;
	nonull_unique_ptr<std::vector<float>> upvec = new std::vector<float>;
	nonull_shared_ptr<std::vector<float>> spvec = new std::vector<float>;

	//nonull<std::unique_ptr<std::unique_ptr<std::unique_ptr<std::vector<float>>>>> mpvec = new std::unique_ptr<std::unique_ptr<std::vector<float>>>(new std::unique_ptr<std::vector<float>>(new std::vector<float>));

	for (std::size_t i = 0; i < N; ++i)
	{
		typedef decltype(+(*pvec)[0]) f_t;
		f_t res = std::cos((f_t)i);

		rvec[i] = res;
		pvec->push_back(res);
		upvec->push_back(res);
		spvec->push_back(res);
	}

	for (std::size_t i = 0; i < 10; ++i) std::cerr << rvec[i] << ", ";
	std::cerr << '\n';

	for (std::size_t i = 0; i < pvec->size(); ++i) std::cerr << (*pvec)[i] << ", ";
	std::cerr << '\n';

	for (std::size_t i = 0; i < upvec->size(); ++i) std::cerr << (*upvec)[i] << ", ";
	std::cerr << '\n';

	for (std::size_t i = 0; i < spvec->size(); ++i) std::cerr << (*spvec)[i] << ", ";
	std::cerr << '\n';

	std::cerr << '\n';

	print_vec(pvec);
	//print_vec(upvec); // error - unique_ptr cannot be copied
	print_vec(std::move(upvec));
	print_vec(spvec);
	
	print_vec_raw1(rvec);
	print_vec_raw2(rvec);

	std::cerr << '\n';

	constexpr nonull<void(*)(const std::string&)> fp1 = print_msg;
	constexpr void(*_fp1)(const std::string&) = print_msg;
	//fp1 = print_msg;

	nonull_ptr<void(const std::string&)> fp2 = print_msg;
	fp2 = print_msg;

	fp1("hello");
	fp2("world");

	std::cerr << '\n';

	std::cin.get();
}
