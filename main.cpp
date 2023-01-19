#include "gtest/gtest.h"
#include "fs.h"
#include "sha256.h"

#include <fstream>
#include <stdio.h>

void duplicate(std::string fsrc, std::string fdest)
{
    std::ifstream  src(fsrc, std::ios::binary);
    std::ofstream  dst(fdest, std::ios::binary);

    dst << src.rdbuf();
}


TEST(FsTest, init021005){
    initFs("fs2-10-5.bin.solucao", 2, 10, 5);
    ASSERT_EQ(printSha256("fs2-10-5.bin.solucao"),std::string("F7:71:A2:19:63:85:52:25:AF:50:89:31:D7:BD:57:9E:BC:5E:3D:A2:85:4F:FE:41:B8:63:1A:5B:18:3F:0E:85"));
}

TEST(FsTest, init011010){
    initFs("fs1-10-10.bin.solucao", 1, 10, 10);
    ASSERT_EQ(printSha256("fs1-10-10.bin.solucao"),std::string("F4:ED:F3:23:45:16:CA:BF:78:1A:BE:6F:EF:DB:7F:0F:BA:07:F5:88:D7:A5:CD:65:1F:18:A4:81:65:91:E3:F4"));
}

TEST(FsTest, case3){
    initFs("fs4-32-16.bin.solucao", 4, 32, 16);
    ASSERT_EQ(printSha256("fs4-32-16.bin.solucao"),std::string("A2:71:21:00:D1:4C:10:94:C9:A0:0A:BD:03:E7:25:38:EA:3E:04:07:57:E4:02:87:5F:7D:1F:B7:35:6D:FE:E4"));
}

TEST(FsTest, case4){
    
    duplicate("fs-case4.bin", "fs-case4.bin.solucao");
    
    addFile("fs-case4.bin.solucao", "/teste.txt", "abc");
    ASSERT_EQ(printSha256("fs-case4.bin.solucao"),std::string("AA:29:B7:CF:09:B6:32:0E:6B:20:51:ED:FD:8E:40:FB:B0:A8:71:FA:8A:22:0A:06:F4:E1:E4:69:0A:C6:B2:77"));
}

TEST(FsTest, case5){
    duplicate("fs-case5.bin", "fs-case5.bin.solucao");
    
    addDir("fs-case5.bin.solucao", "/dec7556");
    ASSERT_EQ(printSha256("fs-case5.bin.solucao"),std::string("0B:BB:60:5C:52:BC:0D:4F:5C:2C:B8:AA:2D:F5:F6:43:7A:EC:02:80:72:F2:D7:C3:7B:91:A6:FE:9E:4C:B6:44"));
    
}

TEST(FsTest, case6){
    duplicate("fs-case6.bin", "fs-case6.bin.solucao");
    
    addFile("fs-case6.bin.solucao", "/dec7556/t2.txt", "fghi");
    ASSERT_EQ(printSha256("fs-case6.bin.solucao"),std::string("C5:D5:15:D8:2F:09:15:49:D9:A2:B5:58:36:E7:DC:28:E5:C4:14:02:1D:03:0E:A8:4E:40:EE:76:BF:05:F0:C6"));
}

TEST(FsTest, case7){
    duplicate("fs-case7.bin", "fs-case7.bin.back");

    remove("fs-case7.bin", "/dec7556/t2.txt");
    ASSERT_EQ(printSha256("fs-case7.bin"),std::string("30:9A:B6:81:77:C1:FF:C9:7A:39:FD:77:DC:98:2A:47:C4:3A:31:A4:79:1A:49:FB:AB:8F:42:4B:66:7D:C0:8D"));

    rename("fs-case7.bin.back", "fs-case7.bin");
}

TEST(FsTest, case8){
    duplicate("fs-case8.bin", "fs-case8.bin.back");

    remove("fs-case8.bin", "/dec7556");
    ASSERT_EQ(printSha256("fs-case8.bin"),std::string("D4:63:6C:09:AD:B9:D3:68:6F:1B:02:79:78:38:50:C2:31:7D:E2:F1:C1:50:C9:13:7D:D9:0A:77:B5:27:4E:36"));

    rename("fs-case8.bin.back", "fs-case8.bin");
}

TEST(FsTest, case9){
    duplicate("fs-case9.bin", "fs-case9.bin.back");
    
    move("fs-case9.bin", "/dec7556/t2.txt", "/t2.txt");
    ASSERT_EQ(printSha256("fs-case9.bin"),std::string("48:D0:98:B2:5F:BF:D8:4B:A6:37:1F:9A:13:8F:C0:D2:2B:6E:21:39:AB:67:15:7F:DF:AE:3E:23:6D:85:49:04"));
    
    rename("fs-case9.bin.back", "fs-case9.bin");
}

TEST(FsTest, case10){
    duplicate("fs-case10.bin", "fs-case10.bin.back");
    
    move("fs-case10.bin", "/teste.txt", "/dec7556/teste.txt");
    ASSERT_EQ(printSha256("fs-case10.bin"),std::string("36:EB:18:B6:6F:9C:1E:20:B1:3A:86:81:A7:9D:0B:2E:A4:B8:A1:8E:92:B1:FB:B3:70:15:E8:9E:48:47:FC:53"));
    
    rename("fs-case10.bin.back", "fs-case10.bin");
}

TEST(FsTest, case11){
    duplicate("fs-case11.bin", "fs-case11.bin.back");
    
    remove("fs-case11.bin", "/a.txt");
    ASSERT_EQ(printSha256("fs-case11.bin"),std::string("FE:4B:F3:7F:F8:14:4C:24:DA:1E:94:04:4E:9B:02:E9:12:F1:23:7A:D3:EC:E6:DC:7A:65:0B:4B:37:A1:2E:30"));
    
    rename("fs-case11.bin.back", "fs-case11.bin");
}

TEST(FsTest, case12){
    duplicate("fs-case12.bin", "fs-case12.bin.back");
    
    move("fs-case12.bin", "/b.txt", "/a.txt");
    ASSERT_EQ(printSha256("fs-case12.bin"),std::string("BC:2B:05:C8:8B:DF:02:41:3B:E3:86:8E:4C:CC:C1:FF:63:87:F9:A5:24:15:16:49:83:88:F0:75:18:D1:1B:BE"));
    
    rename("fs-case12.bin.back", "fs-case12.bin");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}