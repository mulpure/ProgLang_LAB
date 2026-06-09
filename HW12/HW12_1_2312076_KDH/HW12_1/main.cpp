/*
솔루션명 : HW12_1_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - 학생 정보를 텍스트 파일에서 읽어 Student 객체 배열에 저장
    - 각 학생 객체의 해시 코드를 생성
    - 생성된 해시 코드를 이용하여 4비트(16 버킷), 8비트(256 버킷) 해시값 계산
    - 학생 정보와 해시값을 표 형태로 출력


최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
    


#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Student.h"

#define MAX_HASH_4BIT 16
#define MAX_HASH_8BIT 256

int main()
{
    // 입력 파일명
    const std::string fileName = "Students_data.txt";

    // 최대 학생 수
    const int MAX_STUDENTS = 100;

    // 학생 객체 저장 배열
    Student students[MAX_STUDENTS];

    // 입력된 학생 수
    int numStudents = 0;

    // 4비트, 8비트 해시값 저장 변수
    size_t hash_value_4;
    size_t hash_value_8;

    // 입력 파일 열기
    std::ifstream fin(fileName);

    // 파일 열기 실패 검사
    if (!fin)
    {
        std::cerr << "File open error : "
            << fileName << std::endl;
        return 1;
    }

    
    // 파일 끝 또는 최대 학생 수까지 학생 정보를 배열에 저장
    
    while (numStudents < MAX_STUDENTS &&
        (fin >> students[numStudents]))
    {
        ++numStudents;
    }

    // 입력 형식 오류 검사
    if (!fin.eof())
    {
        std::cerr << "Input format error while reading student data."
            << std::endl;
        return 1;
    }

    // 입력된 학생 수 출력
    std::cout << "Number of input students : "
        << numStudents << std::endl;

    // 출력 헤더
    std::cout << std::left
        << std::setw(10) << "StID"
        << std::setw(20) << "Name"
        << std::setw(16) << "Telephone"
        << std::setw(24) << "E-mail"
        << std::setw(20) << "Address"
        << std::setw(16) << "Hash(4bit)"
        << std::setw(16) << "Hash(8bit)"
        << std::endl;

    std::cout << std::string(120, '-') << std::endl;

     // 각 학생의 해시 코드를 계산하고 4비트, 8비트 해시값을 출력
    
    for (int i = 0; i < numStudents; i++)
    {
        size_t hashCode = students[i].getHashCode();

        hash_value_4 = hashCode % MAX_HASH_4BIT;
        hash_value_8 = hashCode % MAX_HASH_8BIT;

        std::cout << students[i] << " "
            << std::setw(16) << hash_value_4
            << std::setw(16) << hash_value_8
            << std::endl;
    }

    std::cout << std::string(120, '-')
        << std::endl;

    return 0;
}