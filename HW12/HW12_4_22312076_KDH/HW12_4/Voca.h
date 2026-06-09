/*
솔루션명 : HW12_4_22312076_KDH
헤 더 명 : Voca.h

프로그램의 목적 및 기능
    - 어휘(Voca) 항목의 키워드, 품사, 유의어, 예문 데이터를 관리
    - 정규식(regex)을 이용해 데이터 파일의 한 줄을 파싱하여 객체로 변환

클래스 목록
    - Voca

특이사항
    - 데이터 파일 형식: keyword, PARTOFSPECH, { "syn1" }, { "usage" };
    - setWordType() 호출 전 공백 제거(trimStr)를 수행하여 파싱 오류 방지

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#ifndef VOCA_H
#define VOCA_H
#include <iostream>
#include <string>
#include <regex>
#include <list>

// 품사 열거형 (NOUN=0, VERB=1, ADJ=2, ADV=3, PREPOS=4, ERROR=5)
enum Word_Type { NOUN, VERB, ADJ, ADV, PREPOS, WORD_TYPE_ERROR };
typedef std::list<std::string> List_Str;
typedef std::list<std::string>::iterator Lst_Str_Itr;

class Voca {
    // 출력 연산자: "keyword(품사): \n - thesaurus(...)\n - example usage(...)" 형식으로 출력
    friend std::ostream& operator<<(std::ostream& fout, const Voca& mv)
    {
        std::string wd_ty[] = { "n", "v", "adj", "adv", "prepos", "error" };
        fout << mv.keyWord << "(" << wd_ty[mv.wordType] << "): \n";
        fout << " - thesaurus(";
        printStringList(fout, mv.thesaurus);
        fout << ")" << std::endl;
        fout << " - example usage(";
        printStringList(fout, mv.usages);
        fout << ")";
        return fout;
    }
private:
    std::string keyWord;
    Word_Type   wordType;
    std::list<std::string> thesaurus;
    std::list<std::string> usages;

    // 문자열 앞뒤 공백 제거 (정규식 캡처 결과의 공백 처리용)
    static std::string trimStr(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

public:
    Voca(std::string kw, Word_Type wt, List_Str thes, List_Str ex_usg)
        : keyWord(kw), wordType(wt), thesaurus(thes), usages(ex_usg) {
    }
    Voca() : wordType(WORD_TYPE_ERROR) {}

    const std::string getKeyWord() const { return keyWord; }
    Word_Type getWordType() const { return wordType; }
    const std::list<std::string>& getThesaurus() const { return this->thesaurus; }
    const std::list<std::string>& getUsages()    const { return this->usages; }

    void setKeyWord(std::string kw) { this->keyWord = trimStr(kw); }
    void setThesaurus(std::list<std::string> thes) { this->thesaurus = thes; }
    void setUsages(std::list<std::string> usages) { this->usages = usages; }

    // 문자열로 받은 품사명을 Word_Type 열거형으로 변환
    void setWordType(std::string wt) {
        wt = trimStr(wt);
        Word_Type wordType;
        if (wt == "NOUN")   wordType = NOUN;
        else if (wt == "VERB")   wordType = VERB;
        else if (wt == "ADJ")    wordType = ADJ;
        else if (wt == "ADV")    wordType = ADV;
        else if (wt == "PREPOS") wordType = PREPOS;
        else {
            std::cout << "Error in word_type : " << wt;
            std::cout << " is not implemented.yet !!!" << std::endl;
            wordType = WORD_TYPE_ERROR;
        }
        this->wordType = wordType;
    }

    // 리스트의 문자열 항목들을 쉼표로 구분하여 출력
    static void printStringList(std::ostream& fout,
        const std::list<std::string>& values)
    {
        bool first = true;
        for (const auto& value : values) {
            if (!first) fout << ", ";
            fout << value;
            first = false;
        }
    }

    // 텍스트에서 "..." 형태의 따옴표 문자열만 추출하여 리스트로 반환
    std::list<std::string> extractQuotedStrings(const std::string& text)
    {
        std::list<std::string> result;
        std::regex itemPattern(R"(\"([^"]*)\")");
        auto begin = std::sregex_iterator(text.begin(), text.end(), itemPattern);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it)
            result.push_back((*it)[1].str());
        return result;
    }

    /*
    * 함수명 : parseLineByRegex
    * 설  명 : 데이터 파일의 한 줄을 정규식으로 파싱하여 멤버 변수에 저장
    *          그룹1=keyword, 그룹2=word_type, 그룹3=thesaurus, 그룹4=usages
    * 매개변수 : line - 파싱할 입력 문자열
    * 반환값 : bool (파싱 성공 여부)
    */
    bool parseLineByRegex(const std::string& line) {
        std::regex linePattern(
            R"(^\s*([^,]+)\s*,\s*([^,]+)\s*,\s*\{([^}]*)\}\s*,?\s*\{([^}]*)\}\s*;\s*$)"
        );
        std::smatch match;
        if (!regex_match(line, match, linePattern))
            return false;
        this->setKeyWord(match[1].str());
        this->setWordType(match[2].str());
        this->thesaurus = extractQuotedStrings(match[3].str());
        this->usages = extractQuotedStrings(match[4].str());
        return true;
    }

    // parseLineByRegex 호출 래퍼 (반환값: 성공=1, 실패=0)
    int fgetVoca(const std::string& line) {
        return parseLineByRegex(line);
    }
};
#endif