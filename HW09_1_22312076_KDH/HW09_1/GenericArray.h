/*
솔루션명 : HW09_1_22312076_KDH
소스코드 : GenericArray.h

프로그램의 목적 및 기능
    - GenericArray 템플릿 클래스 구현
    - 배열 생성, 셔플, 정렬 및 출력 기능 제공
    - 배열 입출력 연산자 구현

함수 목록
    - GenericArray
    - ~GenericArray
    - genBigRandArray
    - setNumElements
    - shuffle
    - insertSort
    - printArray
    - getGA
    - operator>>
    - operator<<

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/

#ifndef GENERICARRAY_H
#define GENERICARRAY_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

//=======템플릿 선언============
template<typename T>
class GenericArray;

template<typename T>
istream& operator>>(istream& fin, GenericArray<T>& ga);

template<typename T>
ostream& operator<<(ostream& fout, const GenericArray<T>& ga);

/*
* 클래스명 : GenericArray
* 설  명 : 템플릿 기반 동적 배열 클래스
*/
template<typename T>
class GenericArray {
    friend istream& operator>> <T>(istream& fin, GenericArray<T>& ga);
    friend ostream& operator<< <T>(ostream& fout, const GenericArray<T>& ga);

public:
    GenericArray(string nm, int capacity = 0);
    ~GenericArray(); //강의시간 설명해주신대로 미작성.

    void genBigRandArray(int size, T offset);
    void setNumElements(int num_elements) { this->num_elements = num_elements; }//원소 개수 설정
    void shuffle();
    void insertSort();
    void printArray(int per_line, int sample_lines);

    T* getGA() { return GA; }//배열 포인터 반환

protected:
    string name;//배열 이름
    T* GA;//동적 배열 포인터
    int num_elements;//현재 원소 개수
    int capacity;//배열 최대 용량
};


//===============템플릿=================
/*
* 함수명 : GenericArray
* 설  명 : GenericArray 객체 초기화
* 매개변수 :
*   - nm : 배열 이름
*   - cap : 배열 최대 크기
* 반환값 : 없음
*/
template<typename T>
GenericArray<T>::GenericArray(string nm, int cap)
    : name(nm), GA(nullptr), num_elements(0), capacity(cap)
{
    if (capacity > 0)
        GA = new T[capacity];//동적 배열 생성
}

/*
* 함수명 : ~GenericArray
* 설  명 : 동적 배열 메모리 해제
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
GenericArray<T>::~GenericArray()
{
    delete[] GA;//배열 메모리 해제
    GA = nullptr;//포인터 초기화
}

/*
* 함수명 : genBigRandArray
* 설  명 : offset 기준의 중복 없는 랜덤 배열 생성
* 매개변수 :
*   - size : 생성할 배열 크기
*   - offset : 시작 값
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::genBigRandArray(int size, T offset)
{
    delete[] GA;//기존 배열 삭제

    capacity = size;//배열 용량 설정
    num_elements = size;//원소 개수 설정

    GA = new T[capacity];//새 배열 생성

    for (int i = 0; i < size; i++)
        GA[i] = static_cast<T>(i) + offset;//배열 값 저장

    shuffle();//배열 섞기
}

/*
* 함수명 : shuffle
* 설  명 : Fisher-Yates 알고리즘 기반 배열 셔플
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::shuffle()
{
    srand(static_cast<unsigned>(time(nullptr)));//난수 시드 설정

    for (int i = num_elements - 1; i > 0; i--) {
        long long range = static_cast<long long>(i) + 1;
        long long j;

        if (range > RAND_MAX) {
            j = (static_cast<long long>(rand()) * (RAND_MAX + 1LL) + rand()) % range;//큰 범위 난수 생성
        }
        else {
            j = rand() % range;//일반 난수 생성
        }

        swap(GA[i], GA[static_cast<int>(j)]);//원소 교환
    }
}

/*
* 함수명 : insertSort
* 설  명 : 삽입 정렬 수행
* 매개변수 : 없음
* 반환값 : 없음
*/
template<typename T>
void GenericArray<T>::insertSort()
{
    for (int i = 1; i < num_elements; i++) {
        T key = GA[i];//삽입할 원소 저장
        int j = i - 1;

        while (j >= 0 && GA[j] > key) {
            GA[j + 1] = GA[j];//원소 이동
            j--;
        }

        GA[j + 1] = key;//원소 삽입
    }
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
void GenericArray<T>::printArray(int per_line, int sample_lines)
{
    cout << fixed << setprecision(2);//소수점 둘째 자리 고정

    int show = min(num_elements, per_line * sample_lines);//출력 개수 계산

    for (int i = 0; i < show; i++) {
        cout << setw(10) << GA[i];

        if ((i + 1) % per_line == 0)
            cout << "\n";//줄바꿈
    }

    if (show % per_line != 0)
        cout << "\n";

    cout.unsetf(ios::fixed);//출력 형식 초기화
}

/*
* 함수명 : operator>>
* 설  명 : 파일 입력 스트림으로 배열 데이터 입력
* 매개변수 :
*   - fin : 입력 스트림
*   - ga : 입력 대상 GenericArray 객체
* 반환값 : 입력 스트림
*/
template<typename T>
istream& operator>>(istream& fin, GenericArray<T>& ga)
{
    fin >> ga.name >> ga.num_elements;//배열 이름 및 원소 개수 입력

    delete[] ga.GA;//기존 배열 삭제

    ga.capacity = ga.num_elements;//배열 용량 설정
    ga.GA = new T[ga.capacity];//새 배열 생성

    for (int i = 0; i < ga.num_elements; i++)
        fin >> ga.GA[i];//배열 데이터 입력

    return fin;
}

/*
* 함수명 : operator<<
* 설  명 : 배열 데이터를 출력 스트림으로 출력
* 매개변수 :
*   - fout : 출력 스트림
*   - ga : 출력 대상 GenericArray 객체
* 반환값 : 출력 스트림
*/
template<typename T>
ostream& operator<<(ostream& fout, const GenericArray<T>& ga)
{
    fout << ga.name << " " << ga.num_elements << "\n";

    for (int i = 0; i < ga.num_elements; i++) {
        fout << ga.GA[i];

        if (i + 1 < ga.num_elements)
            fout << " ";//원소 구분 공백 출력
    }

    fout << "\n";

    return fout;
}

#endif