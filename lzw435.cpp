/*
    Author: Nicholas Kucek
    3460:435 Algorithms - Project 2: LZW - Part 1
    lzw435.cpp - main file

    This code is derived in parts from LZW@RosettaCode for UA CS435.
    Link: https://rosettacode.org/wiki/LZW_compression#C.2B.2B

    Compress a string to a list of output symbols.
    The result will be written to the output iterator
    starting at "result"; the final iterator is returned.
*/

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sys/stat.h>

template <typename Iterator> Iterator compress(const std::string &uncompressed, Iterator result) {

    // Build the dictionary, start with 256.
    int dictSize = 256;
    std::map<std::string,int> dictionary;
    for (int i = 0; i < 256; i++)
        dictionary[std::string(1, i)] = i;

    std::string w;
    for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) {
        char c = *it;
        std::string wc = w + c;
        if (dictionary.count(wc)) w = wc;
        else {
            *result++ = dictionary[w];
            // Add wc to the dictionary. Assuming the size is 4096!!!
            if (dictionary.size() < 4096) dictionary[wc] = dictSize++;
            w = std::string(1, c);
        }
    }

    // Output the code for w.
    if (!w.empty())
      *result++ = dictionary[w];
    return result;
}

 
// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator> std::string decompress(Iterator begin, Iterator end) {

    // Build the dictionary.
    int dictSize = 256;
    std::map<int,std::string> dictionary;
    for (int i = 0; i < 256; i++)
        dictionary[i] = std::string(1, i);
    std::string w(1, *begin++);
    std::string result = w;
    // comment out this line of code to decompress larger files (i.e. bible2.txt) within seconds
    std::cout << "decompressed: " << result;
    std::string entry;
    for ( ; begin != end; begin++) {

        int k = *begin;
        if (dictionary.count(k)) entry = dictionary[k];
        else if (k == dictSize) entry = w + w[0];
        else throw "Bad compressed k";

        result += entry;
        // comment out this line of code as well to decompress larger files (i.e. bible2.txt) within seconds
        std::cout << "\ndecompressed: " << result;

        // Add w+entry[0] to the dictionary.
        if (dictionary.size()<4096) dictionary[dictSize++] = w + entry[0];
        w = entry;
    }

    return result;
}

// convert an integer into a string of bits
std::string int2BinaryString(int c, int cl) {

    std::string p = ""; // a binary code string with code length = cl
    int code = c;
    while (c > 0) {
	    if (c % 2 == 0)
              p = "0" + p;
          else
              p = "1" + p;
       c = c >> 1;
    }
    int zeros = cl - p.size();
    if (zeros < 0) {
       std::cout << "\nWarning: Overflow. code " << code << " is too big to be coded by " << cl << " bits!\n";
       p = p.substr(p.size() - cl);
    }
    else {
        for (int i = 0; i < zeros; i++)  // pad 0s to left of the binary code if needed
            p = "0" + p;
    }

    return p;
}

// convert a string of bits into an integer
int binaryString2Int(std::string p) {

    int code = 0;
    if (p.size() > 0) {
        if (p.at(0) == '1') code = 1;
        p = p.substr(1);
        while (p.size() > 0) {
            code = code << 1;
            if (p.at(0) == '1') code++;
            p = p.substr(1);
       }
    }

    return code;
}

void binaryIODemo(std::vector<int> compressed) {

    // example for integer c; looking for a 9 bit code
    int c = 69;
    int bits = 9; // length of the code
    std::string p = int2BinaryString(c, bits);
    std::cout << "c=" << c << " : binary string=" << p << "; back to code=" << binaryString2Int(p) << "\n";

    std::string bcode= "";
    for (std::vector<int>::iterator it = compressed.begin() ; it != compressed.end(); ++it) {

        if (*it < 256)
             bits = 8;
        else
             bits = 9;

        // assuming 12 bits
        bits = 12;
        p = int2BinaryString(*it, bits);
        std::cout << "c=" << *it << " : binary string="<<p<<"; back to code=" << binaryString2Int(p) << "\n";
        bcode+=p;
    }

    // writing to file
    std::cout << "string 2 save : "<< bcode << "\n";
    std::string fileName = "example435.lzw"; // this is for demo you should not hard code the final name.
    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ios::binary);

    std::string zeros = "00000000";
    if (bcode.size() % 8 != 0) // make sure the length of the binary string is a multiple of 8
        bcode += zeros.substr(0, 8-bcode.size()%8);

    int b;
    for (int i = 0; i < bcode.size(); i+=8) {
        b = 1;
        for (int j = 0; j < 8; j++) {
            b = b << 1;
            if (bcode.at(i+j) == '1') b+=1;
        }
        char c = (char) (b & 255); // save the string byte by byte
        myfile.write(&c, 1);
    }
    myfile.close();

    // reading from a file
    std::ifstream myfile2;
    myfile2.open(fileName.c_str(),  std::ios::binary);

    struct stat filestatus;
    stat(fileName.c_str(), &filestatus);
    long fsize = filestatus.st_size; // get the size of the file in bytes

    char c2[fsize];
    myfile2.read(c2, fsize);

    std::string s = "";
    long count = 0;
    while(count < fsize) {
       unsigned char uc =  (unsigned char) c2[count];
       std::string p = ""; // a binary string
       for (int j = 0; j < 8 && uc > 0; j++) {
            if (uc % 2 == 0) p = "0" + p;
            else p = "1" + p;
            uc = uc >> 1;
       }
       p = zeros.substr(0, 8 - p.size()) + p; // pad 0s to left if needed
       s += p;
       count++;
    }

    myfile2.close();
    std::cout << " saved string : " << s << "\n";
}

// returns the size of a file (used for self-assessment)
int get_file_size(std::string filename) { // path to file
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

// demo of how LZW works
int main(int argc, char** argv) {

    try {

        std::string filename, binarybits, filecontents, decompressed;
        std::ifstream testCase;
        std::ofstream testCaselzw, testCaseOutput;
        std::stringstream buffer;
        std::vector<int> compressed;

        // file compression
        if(argc == 3 && argv[1][0] == 'c') {

            filename = argv[2];
            testCase.open(filename);
            if(testCase.fail()) throw "Error: file could not opened or does not exist";
            std::string newfilename = filename.substr(0, filename.find_last_of('.')) + ".lzw";
            testCaselzw.open(newfilename);
            if(testCaselzw.fail()) throw "Error: file could not opened or does not exist";
            std::string filecontents;

            if (testCase) {

              buffer << testCase.rdbuf();
              testCase.close();
              filecontents = buffer.str();
            }

            compress(filecontents, std::back_inserter(compressed));
            for (auto itr = compressed.begin(); itr != compressed.end(); itr++) {

                binarybits = int2BinaryString(*itr, 12);
                testCaselzw << binarybits;
            }

            testCaselzw.close();
        }

        // file decompression
        if(argc==3 && argv[1][0] == 'e') {

            filename = argv[2];
            std::string newfilename = filename.substr(0, filename.find_last_of('.'));
            testCase.open(filename);
            if(testCase.fail()) throw "Error: file could not opened or does not exist";
            testCaseOutput.open(newfilename+"_2");
            if(testCaseOutput.fail()) throw "Error: file could not opened or does not exist";

            if(testCase) {

                buffer << testCase.rdbuf();
                filecontents = buffer.str();
                testCase.close();
            }

            const char* array = filecontents.c_str();
            for(int i = 0; i < filecontents.length(); i+=12) {

                for(int j = 0; j < 12; j++) {

                    binarybits += array[i+j];
                }

                compressed.push_back(binaryString2Int(binarybits));
                binarybits.clear();
            }

            decompressed = decompress(compressed.begin(), compressed.end());
            std::cout << "\nfinal decompressed: " << decompressed << std::endl;
            testCaseOutput << decompressed;
            testCaseOutput.close();
        }

        //demo as the name suggests
        binaryIODemo(compressed);
    } catch (char const* err) {
        std::cout << "\nThe library threw an exception:\n"
            << err << std::endl;
    }

    return 0;
}