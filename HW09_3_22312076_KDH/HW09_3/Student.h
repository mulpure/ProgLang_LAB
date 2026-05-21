/*
솔루션명 : HW09_3_22312076_KDH
소스코드 : Student.h

프로그램의 목적 및 기능
    - Student 클래스 선언
    - 학생 정보 저장 및 출력 기능 정의
    - priority_queue 정렬을 위한 비교 클래스 제공

함수 목록
    - Student
    - print
    - operator()

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/

#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

//Student 정렬 기준 정의

enum class KeyAttr { NAME, STID, GPA };

// 정렬 방식 정의
enum class SortOrder { ASC, DESC };

//학생 정보 저장 클래스
class Student {
public:
    Student(const string& name, int stID, double GPA);
    void print() const;

    string name;
    int stID;
    double GPA;
};

/*
* 클래스명 : StudentCompare
* 설  명 : priority_queue 정렬 비교 클래스
*/
class StudentCompare {
public:

    /*
    * 함수명 : StudentCompare
    * 설  명 : 비교 기준 및 정렬 방식 초기화
    * 매개변수 :
    *   - keyAttr : 정렬 기준
    *   - order : 정렬 방식
    * 반환값 : 없음
    */
    StudentCompare(KeyAttr keyAttr, SortOrder order)
        : keyAttr_(keyAttr), order_(order) {
    }

    /*
    * 함수명 : operator()
    * 설  명 : priority_queue 우선순위 비교 수행
    * 매개변수 :
    *   - a : 비교 대상 Student 객체
    *   - b : 비교 대상 Student 객체
    * 반환값 : 우선순위 비교 결과
    */
    bool operator()(const Student& a, const Student& b) const {
        switch (keyAttr_) {

        case KeyAttr::NAME://이름 기준 비교
            return (order_ == SortOrder::ASC) ? (a.name > b.name): (a.name < b.name);

        case KeyAttr::STID://학번 기준 비교
            return (order_ == SortOrder::ASC) ? (a.stID > b.stID) : (a.stID < b.stID);

        case KeyAttr::GPA://학점 기준 비교
            return (order_ == SortOrder::ASC) ? (a.GPA > b.GPA) : (a.GPA < b.GPA);
        }

        return false;
    }

private:
    KeyAttr keyAttr_;
    SortOrder order_;
};

/*
* 함수명 : Student
* 설  명 : Student 객체 초기화
* 매개변수 :
*   - name : 학생 이름
*   - stID : 학번
*   - GPA : 학점
* 반환값 : 없음
*/
Student::Student(const string& name, int stID, double GPA) : name(name), stID(stID), GPA(GPA) {
}

/*
* 함수명 : print
* 설  명 : 학생 정보 출력
* 매개변수 : 없음
* 반환값 : 없음
*/
void Student::print() const {
    cout << left << setw(12) << name << right << setw(10) << stID
        << setw(8) << fixed << setprecision(2) << GPA << endl;
}


#endif

