/*
솔루션명 : HW09_2_22312076_KDH
소스코드 : GenericCirQ.h

프로그램의 목적 및 기능
    - GenericCirQ 템플릿 클래스 구현
    - 원형 큐의 enqueue, dequeue 기능 제공
    - 큐 상태 출력 기능 제공

함수 목록
    - GenericCirQ
    - ~GenericCirQ
    - enqueue
    - dequeue
    - isEmpty
    - isFull
    - size
    - fprint
    - operator<<

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/13
*/

#ifndef GENERIC_CIRQ_H
#define GENERIC_CIRQ_H

#include "GenericArray.h"
#include <iostream>
#include <string>

template<typename T>
class GenericCirQ;

template<typename T>
std::ostream& operator<<(std::ostream& fout, const GenericCirQ<T>& gCirQ);

/*
* 클래스명 : GenericCirQ
* 설  명 : GenericArray를 기반으로 구현한 원형 큐 클래스
*/
template<typename T>
class GenericCirQ : public GenericArray<T>
{
    friend std::ostream& operator<< <T>(std::ostream& fout,
        const GenericCirQ<T>& gCirQ);

public:
    GenericCirQ(std::string nm, int capacity);
    ~GenericCirQ() {}

    T* dequeue();//큐에서 데이터 삭제
    T* enqueue(const T element);//큐에 데이터 삽입

    //공백 여부 확인
    bool isEmpty() { 
        return (this->num_elements == 0); 
    }
    //가득 찼는지 확인
    bool isFull() { 
        return (this->num_elements >= this->capacity); 
    }
    //현재 원소 개수 반환
    int size() { 
        return this->num_elements;
    }

    void fprint(std::ostream& fout, int elements_per_line);

private:
    int front;//큐의 front 인덱스
    int back;//큐의 back 인덱스
};

/*
* 함수명 : GenericCirQ
* 설  명 : 원형 큐 객체 초기화
* 매개변수 :
*   - nm : 큐 이름
*   - capacity : 큐 최대 크기
* 반환값 : 없음
*/
template<typename T>
GenericCirQ<T>::GenericCirQ(std::string nm, int capacity) : GenericArray<T>(nm, capacity), front(0), back(0) {}

/*
* 함수명 : enqueue
* 설  명 : 큐에 원소 삽입
* 매개변수 :
*   - element : 삽입할 데이터
* 반환값 : 삽입된 원소 포인터
*/
template<typename T>
T* GenericCirQ<T>::enqueue(const T element)
{
    if (isFull()) {
        std::cout << this->name << " : CirQ is full!" << std::endl;
        return nullptr;
    }

    this->GA[back] = element;//현재 back 위치에 데이터 저장
    back = (back + 1) % this->capacity;//back 인덱스 순환 증가
    this->num_elements++;

    int insertedIdx = (back - 1 + this->capacity) % this->capacity;//삽입 위치 계산

    return &this->GA[insertedIdx];//삽입된 원소 주소 반환
}

/*
* 함수명 : dequeue
* 설  명 : 큐에서 원소 삭제
* 매개변수 : 없음
* 반환값 : 삭제된 원소 포인터
*/
template<typename T>
T* GenericCirQ<T>::dequeue()
{
    if (isEmpty()) {
        std::cout << this->name << " : CirQ is empty!!" << std::endl;
        return nullptr;
    }

    T* ret = &this->GA[front];//삭제할 원소 주소 저장
    front = (front + 1) % this->capacity;//front 인덱스 순환 증가
    this->num_elements--;//원소 개수 감소

    return ret;
}

/*
* 함수명 : fprint
* 설  명 : 큐 상태 출력
* 매개변수 :
*   - fout : 출력 스트림
*   - elements_per_line : 한 줄당 출력 개수
* 반환값 : 없음
*/
template<typename T>
void GenericCirQ<T>::fprint(std::ostream& fout, int elements_per_line)
{
    fout << this->name
        << "(capacity= " << this->capacity
        << ", current num_elements = " << this->num_elements << ") : ";

    if (this->num_elements == 0) {
        fout << "CirQ is empty now" << std::endl;
        return;
    }

    for (int i = 0; i < this->num_elements; i++) {
        if (i > 0)
            fout << ", ";

        fout << this->GA[(front + i) % this->capacity];//현재 큐 데이터 출력
    }

    fout << std::endl;
}

/*
* 함수명 : operator<<
* 설  명 : GenericCirQ 객체 정보 출력
* 매개변수 :
*   - fout : 출력 스트림
*   - gCirQ : 출력 대상 GenericCirQ 객체
* 반환값 : 출력 스트림
*/
template<typename T>
std::ostream& operator<<(std::ostream& fout, const GenericCirQ<T>& gCirQ)
{
    fout << gCirQ.name
        << "(capacity= " << gCirQ.capacity
        << ", current num_elements = " << gCirQ.num_elements << ") : ";

    if (gCirQ.num_elements == 0) {
        fout << "CirQ is empty" << std::endl;
        return fout;
    }

    for (int i = 0; i < gCirQ.num_elements; i++) {
        if (i > 0)
            fout << ", ";

        fout << gCirQ.GA[(gCirQ.front + i) % gCirQ.capacity];//현재 큐 데이터 출력
    }

    fout << std::endl;

    return fout;
}

#endif