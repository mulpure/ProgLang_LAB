/*
솔루션명 : HW09_2_22312076_KDH
소스코드 : GenericArray.h

프로그램의 목적 및 기능
    - GenericArray 템플릿 클래스 구현
    - 배열 생성, 셔플, 정렬 및 출력 기능 제공
    - 배열 입출력 연산자 구현

함수 목록
    - GenericArray
    - ~GenericArray
    - genBigRandArray
    - shuffle
    - insertSort
    - printArray
    - operator>>
    - operator<<

특이사항 : 솔루션 HW09_1_22312076_KDH 내에 헤더파일에서 발췌

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/13
*/

#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

//===============템플릿 선언=============
template<typename T>
class GenericArray;

template<typename T>
std::istream& operator>>(std::istream& fin, GenericArray<T>& ga);

template<typename T>
std::ostream& operator<<(std::ostream& fout, const GenericArray<T>& ga);

template<typename T>
class GenericArray {
    friend std::istream& operator>> <T>(std::istream& fin,
        GenericArray<T>& ga);
    friend std::ostream& operator<< <T>(std::ostream& fout,
        const GenericArray<T>& ga);

public:
    GenericArray(std::string nm, int capacity = 0);
    ~GenericArray();

    void genBigRandArray(int size, T offset);
    void shuffle();
    void insertSort();
    void printArray(int per_line, int sample_lines);

    T* getGA() { return GA; }//배열 포인터 반환

protected:
    std::string name;//배열 이름
    T* GA;//동적 배열 포인터
    int num_elements;//현재 저장된 원소 개수
    int capacity;//배열 최대 용량
};


//=============템플릿=============

/*
* 함수명 : GenericArray
* 설  명 : GenericArray 객체 초기화
* 매개변수 :
*   - nm : 배열 이름
*   - capacity : 배열 최대 크기
* 반환값 : 없음
*/
template<typename T>
GenericArray<T>::GenericArray(std::string nm, int capacity) {
    this->name = nm;//배열 이름 저장
    this->capacity = capacity;//배열 용량 저장
    this->num_elements = 0;//초기 원소 개수 설정
    this->GA = new T[this->capacity]();//동적 배열 생성
}

/*
* 함수명 : ~GenericArray
* 설  명 : 동적 배열 메모리 해제
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
GenericArray<T>::~GenericArray() {
    if (this->GA != NULL)
        delete[] this->GA;//동적 배열 삭제
}

/*
* 함수명 : shuffle
* 설  명 : 배열 원소 순서를 무작위로 섞음
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::shuffle() {
    int uint30, j;
    T temp;

    for (int i = 0; i < this->num_elements; i++) {
        uint30 = (unsigned int)(rand() << 15) | rand();//30비트 난수 생성
        j = uint30 % this->num_elements;//교환할 인덱스 계산

        if (i == j)
            continue;

        temp = this->GA[i];//현재 원소 임시 저장
        this->GA[i] = this->GA[j];//원소 교환
        this->GA[j] = temp;
    }
}


/*
* 함수명 : genBigRandArray
* 설  명 : offset을 기준으로 큰 랜덤 배열 생성
* 매개변수 :
*   - size : 생성할 배열 크기
*   - offset : 시작 값
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::genBigRandArray(int size, T offset) {
    if (size > this->capacity) {
        if (this->GA != NULL)
            delete[] this->GA;//기존 배열 삭제

        this->GA = new T[size]();//새 배열 생성
        this->capacity = size;//배열 용량 갱신
    }

    this->num_elements = size;//원소 개수 저장

    for (int i = 0; i < this->num_elements; i++) {
        this->GA[i] = (T)i + offset;//배열 값 저장
    }

    shuffle();//배열 섞기
}

/*
* 함수명 : insertSort
* 설  명 : 삽입 정렬 수행
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::insertSort() {
    T keyEntry;
    int j;

    for (int k = 1; k < this->num_elements; k++) {
        keyEntry = this->GA[k];//삽입할 원소 저장

        for (j = k; j > 0; j--) {
            if (this->GA[j - 1] <= keyEntry) {//정렬 위치 확인
                break;
            }
            else {
                this->GA[j] = this->GA[j - 1];//원소 이동
            }
        }

        this->GA[j] = keyEntry;//원소 삽입
    }
}


/*
* 함수명 : operator>>
* 설  명 : 입력 스트림으로 배열 정보 입력
* 매개변수 :
*   - fin : 입력 스트림
*   - ga : 입력 대상 GenericArray 객체
* 반환값 : 입력 스트림
*/
template<typename T>
std::istream& operator>>(std::istream& fin, GenericArray<T>& ga)
{
    std::string nm;
    int num_elements;
    T data;

    fin >> nm >> num_elements;

    ga.name = nm;//배열 이름 저장
    ga.num_elements = num_elements;//원소 개수 저장

    if (num_elements > ga.capacity) {
        delete[] ga.GA;//기존 배열 삭제
        ga.GA = new T[ga.num_elements]();//새 배열 생성
        ga.capacity = num_elements;//배열 용량 갱신
    }

    for (int i = 0; i < num_elements; i++) {
        fin >> data;//데이터 입력
        ga.GA[i] = data;//배열 저장
    }

    return fin;
}

/*
* 함수명 : operator<<
* 설  명 : GenericArray 객체 정보 출력
* 매개변수 :
*   - fout : 출력 스트림
*   - ga : 출력 대상 GenericArray 객체
* 반환값 : 출력 스트림
*/
template<typename T>
std::ostream& operator<<(std::ostream& fout, const GenericArray<T>& ga)
{
    fout << ga.name << "(num_elements = " << ga.num_elements << ") = "
        << std::endl;

    for (int i = 0; i < ga.num_elements; i++) {
        fout << ga.GA[i];

        if ((i + 1) % 10 == 0)
            fout << std::endl;//10개 단위 줄바꿈
    }

    fout << std::endl;

    return fout;
}

/*
* 함수명 : printArray
* 설  명 : 배열 일부를 형식에 맞춰 출력
* 매개변수 :
*   - per_line : 한 줄당 출력 개수
*   - sample_lines : 출력할 줄 수
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::printArray(int per_line, int sample_lines) {
    int count = 0;
    int block_size = per_line * sample_lines;//출력 블록 크기

    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpoint);
    std::cout.precision(2);

    for (int i = 0; (i < block_size) && (count < this->num_elements); i++) {
        std::cout << std::setw(8) << this->GA[count];

        if (count >= this->num_elements)
            break;

        if ((i + 1) % per_line == 0)
            std::cout << std::endl;

        count++;
    }

    if (count >= this->num_elements)
        return;

    if ((this->num_elements - block_size) > count) {
        count = this->num_elements - block_size;
        std::cout << "\n        ......" << std::endl;
    }

    for (int i = 0; (i < block_size) && (count < this->num_elements); i++) {
        std::cout << std::setw(8) << this->GA[count];

        if (count >= this->num_elements)
            break;

        if ((i + 1) % per_line == 0)
            std::cout << std::endl;

        count++;
    }
}

#endif