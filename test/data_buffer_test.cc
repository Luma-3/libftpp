#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "data_buffer.hpp"

TEST(DataBufferTest, BasicOperations)
{
    DataBuffer db;

    int32_t int_val = 42;

    db << int_val;

    int32_t result_int = 0;

    db >> result_int;

    EXPECT_EQ(result_int, int_val);
}

TEST(DataBufferTest, MultipleTypes)
{
    DataBuffer db;

    int32_t int_val   = 42;
    float   float_val = 3.14f;
    char    char_val  = 'A';

    db << int_val << float_val << char_val;

    int32_t result_int   = 0;
    float   result_float = 0.0f;
    char    result_char  = '\0';

    db >> result_int >> result_float >> result_char;

    EXPECT_EQ(result_int, int_val);
    EXPECT_FLOAT_EQ(result_float, float_val);
    EXPECT_EQ(result_char, char_val);
}

TEST(DataBufferTest, stringType)
{
    DataBuffer db;

    std::string str_val = "Hello, World!";

    db << str_val;

    std::string result_str;

    db >> result_str;

    EXPECT_EQ(result_str, str_val);
}

TEST(DataBufferTest, StringAndOther)
{
    DataBuffer db;

    std::string str_val = "Hello, World!";
    float       f_val   = 3.14;

    db << str_val << f_val;

    std::string res_str = "";
    float       res_f   = 0;

    db >> res_str >> res_f;
    EXPECT_EQ(res_str, str_val);
    EXPECT_FLOAT_EQ(res_f, f_val);
}

TEST(DataBufferTest, customStruct)
{
    struct CustomStruct
    {
        int32_t id;
        float   value;

        bool operator==(const CustomStruct& other) const
        {
            return id == other.id && value == other.value;
        }
    };

    DataBuffer db;

    CustomStruct cs_val = {1, 2.5f};

    db << cs_val.id << cs_val.value;

    CustomStruct result_cs = {0, 0.0f};

    db >> result_cs.id >> result_cs.value;

    EXPECT_EQ(result_cs, cs_val);
}

TEST(DataBufferTest, VectorOfInts)
{
    DataBuffer           db;
    std::vector<int32_t> vec_val = {1, 2, 3, 4, 5};

    db << vec_val;

    std::vector<int32_t> result_vec;

    db >> result_vec;

    EXPECT_EQ(result_vec, vec_val);
}

TEST(DataBufferTest, VectorOfCustomStructs)
{
    struct CustomStruct
    {
        int32_t id;
        float   value;

        bool operator==(const CustomStruct& other) const
        {
            return id == other.id && value == other.value;
        }
    };

    DataBuffer db;

    std::vector<CustomStruct> vec_val = {{1, 1.1f}, {2, 2.2f}, {3, 3.3f}};

    for (const auto& cs : vec_val)
    {
        db << cs.id << cs.value;
    }

    std::vector<CustomStruct> result_vec;

    for (size_t i = 0; i < vec_val.size(); ++i)
    {
        CustomStruct cs;
        db >> cs.id >> cs.value;
        result_vec.push_back(cs);
    }

    EXPECT_EQ(result_vec, vec_val);
}

TEST(DataBufferTest, VectorAndString)
{
    DataBuffer           db;
    std::vector<int32_t> vec_val = {1, 2, 3, 4, 5};
    std::string          str_val = "Test String";

    db << vec_val << str_val;

    std::vector<int32_t> result_vec;
    std::string          result_str;

    db >> result_vec >> result_str;

    EXPECT_EQ(result_vec, vec_val);
    EXPECT_EQ(result_str, str_val);
}

TEST(DataBufferTest, ClearBuffer)
{
    DataBuffer db;

    int32_t int_val = 42;

    db << int_val;

    db.clear();

    int32_t result_int = 0;

    EXPECT_THROW(db >> result_int, std::runtime_error);
}
