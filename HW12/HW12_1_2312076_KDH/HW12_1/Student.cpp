/*
솔루션명 : HW12_1_22312076_KDH
소스코드 : Student.cpp

프로그램의 목적 및 기능
    - Student 클래스의 멤버 함수 구현
    - 학생 정보 파일 입력 및 출력 기능 제공
    - 학생 객체의 해시 코드 생성 기능 구현

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/

#include "Student.h"

#include <iomanip>
#include <regex>
#include <functional>

#define PRIME_NUMBER 0x9e3779b9

/*
    학생 객체를 기본값으로 초기화
*/
Student::Student()
    : stID(0),
    name(""),
    tel_no{ 0,0,0 },
    email(""),
    addr("")
{}

/*
    전달받은 학생 정보로 객체 초기화

    stID  : 학번
    name  : 이름
    telNo : 전화번호
    email : 이메일
    addr  : 주소
*/
Student::Student(
    int stID,
    const std::string& name,
    Tel_No telNo,
    const std::string& email,
    const std::string& addr)
    : stID(stID),
    name(name),
    tel_no(telNo),
    email(email),
    addr(addr)
{}

/*
    한 줄의 학생 정보를 읽어 Student 객체에 저장

    입력 형식 : 학번 "이름" 전화번호 이메일 "주소"

    반환값 : 입력 스트림 참조
*/
std::istream& operator>>(std::istream& fin, Student& st)
{
    std::string line;

    if (!std::getline(fin, line))
        return fin;

    const std::regex studentPattern(
        R"REGEX(^\s*(\d{8})\s+"([^"]+)"\s+(\d{2,3})-(\d{3,4})-(\d{4})\s+([A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,})\s+"([^"]+)"\s*$)REGEX"
    );

    std::smatch match;

    // 정규식 형식 검사
    if (!std::regex_match(line, match, studentPattern))
    {
        fin.setstate(std::ios::failbit);
        return fin;
    }

    // 학생 정보 저장
    st.stID = std::stoi(match[1].str());
    st.name = match[2].str();

    st.tel_no.region_no = std::stoi(match[3].str());
    st.tel_no.switch_no = std::stoi(match[4].str());
    st.tel_no.line_no = std::stoi(match[5].str());

    st.email = match[6].str();
    st.addr = match[7].str();

    return fin;
}

/*
    학생 객체 정보를 지정된 형식으로 출력

    반환값 :
        출력 스트림 참조
*/
std::ostream& operator<<(std::ostream& fout, const Student& st)
{
    fout << std::left
        << std::setw(10) << st.stID
        << std::setw(20) << st.name
        << std::setfill('0') << std::right
        << std::setw(3) << st.tel_no.region_no
        << '-'
        << std::setw(4) << st.tel_no.switch_no
        << '-'
        << std::setw(4) << st.tel_no.line_no
        << std::setfill(' ') << std::left
        << " "
        << std::setw(24) << st.email
        << std::setw(20) << st.addr;

    return fout;
}

/*
    새로운 값을 기존 해시값에 결합

    seed  : 누적 해시값
    value : 해시 계산 대상 데이터
*/
template<typename T>
void hash_combine(size_t& seed, const T& value)
{
    std::hash<T> hasher;

    seed ^= hasher(value)
        + PRIME_NUMBER
        + (seed << 6)
        + (seed >> 2);
}

/*
    학생 객체의 해시 코드 생성

    사용 항목 :
        - 학번
        - 이름
        - 전화번호

    반환값 :
        생성된 해시 코드
*/
size_t Student::getHashCode()
{
    size_t hash_code = 0;

    hash_combine(hash_code, stID);
    hash_combine(hash_code, name);
    hash_combine(hash_code, tel_no.region_no);
    hash_combine(hash_code, tel_no.switch_no);
    hash_combine(hash_code, tel_no.line_no);

    return hash_code;
}