/*
솔루션명 : HW12_1_22312076_KDH
소스코드 : Student.h

프로그램의 목적 및 기능
    - 학생 정보를 저장하는 Student 클래스 정의
    - 학생 정보 입출력 연산자 선언
    - 학생 객체의 해시 코드 생성 기능 제공

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/


#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>

/*
    전화번호를 저장하기 위한 구조체

    region_no : 지역번호
    switch_no : 국번
    line_no   : 가입자 번호
*/
typedef struct
{
    int region_no;
    int switch_no;
    int line_no;
} Tel_No;

class Student
{
    friend std::ostream& operator<<(std::ostream& fout, const Student& st);
    friend std::istream& operator>>(std::istream& fin, Student& st);

public:
    /*
        학생 객체를 기본값으로 초기화
    */
    Student();

    /*
        학생 정보를 이용하여 객체 생성

        stID  : 학번
        name  : 이름
        telNo : 전화번호
        email : 이메일
        addr  : 주소
    */
    Student(
        int stID,
        const std::string& name,
        Tel_No telNo,
        const std::string& email,
        const std::string& addr);

    
     //학생 객체의 해시 코드 생성
    size_t getHashCode();

private:
    int stID;
    std::string name;
    Tel_No tel_no;
    std::string email;
    std::string addr;
};

#endif