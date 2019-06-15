
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <thread>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

std::string dir_name = "data";
std::string out_name = "out.bin";

size_t batch_size = 10000;      

void MergeFile(const std::string file_1, const std::string file_2, const std::string file_out ){
    
    std::ifstream in_1(file_1, std::ios::in | std::ios::binary);
    std::ifstream in_2(file_2, std::ios::in | std::ios::binary);
    std::ofstream out(file_out, std::ios::out | std::ios::binary);

    std::vector<uint64_t> vec1{std::istream_iterator<uint64_t>{in_1}, {}};
    std::vector<uint64_t> vec2{std::istream_iterator<uint64_t>{in_2}, {}};
    std::vector<uint64_t> merge_vec(vec1.size() + vec2.size());

    std::merge(vec1.begin(), vec1.end(),
               vec2.begin(), vec2.end(),
               merge_vec.begin());

    std::copy(merge_vec.begin(), merge_vec.end(),
              std::ostream_iterator<uint64_t>(out, "\n"));

    std::remove(file_1.c_str());
    std::remove(file_2.c_str());
}

void Sorting(const char* file, size_t num) {
    
    std::vector<uint64_t> vec(batch_size);

    std::ifstream in(file, std::ios::in | std::ios::binary);

    uint64_t start = 0;

    if (num == 1)
        start = 1;

    uint64_t end = start;

    uint64_t k = 0;
    uint64_t tmp = 0;
    std::vector<uint64_t> v;
    std::string t_id = std::to_string(num) + "_";

    while(!in.eof())
    {
        in.seekg(sizeof(uint64_t) * batch_size * end);
        k = 0;

        while ((!in.eof()) && (k < batch_size))
        {
            in.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
            v.push_back(tmp); 
            k++;
        }

        std::sort(v.begin(), v.end());

        std::ofstream out(dir_name + "/" + t_id + std::to_string(end) + ".bin",
            std::ios::out | std::ios::binary);

        for (uint64_t value : v) 
            out << value << " ";

        v.clear();

        end += 2;
    }


    std::string file_1 = dir_name + "/" + t_id + std::to_string(start) + ".bin";
    std::string file_2 = "";
    std::string file_3 = "";

    // Пока не успеваю, но слияние можно сделать более равномерным, чем будет дальше.

    while (end - start > 1)
    {    
        file_2 = dir_name + "/" + t_id + std::to_string(start + 2) + ".bin";
        file_3 = dir_name + "/" + t_id + std::to_string(start + 2) + "_merged.bin"; 

        MergeFile(file_1, file_2, file_3);

        start += 2;
        file_1 = dir_name + "/" + t_id + std::to_string(start) + "_merged.bin";

        if (start > 3)
            std::remove((dir_name + "/" + t_id + std::to_string(start - 2) + "_merged.bin").c_str());
    }

    std::rename((file_1).c_str(), (dir_name + "/" + t_id + "result.bin").c_str());
}

int main(int argc, char* argv[]) 
{
    if (argc != 2 ) 
    {
        std::cout << "No input file name!" << std::endl;
        return 0;
    }

    mkdir(dir_name.c_str(), ACCESSPERMS);

    std::thread t0(Sorting, argv[1], 0);
    std::thread t1(Sorting, argv[1], 1);

    t0.join();
    t1.join();

    MergeFile(dir_name + "/0_result.bin", dir_name + "/1_result.bin", out_name);

    rmdir(dir_name.c_str());

    return 0;
}