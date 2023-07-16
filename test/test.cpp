#include <zz_logger.h>

#include <iostream>

int main() {
    SET_PROJECT_NAME("zz_logger_test")
	LOG_E << "asfdf";
	std::cout << "asasdfasfd" << std::endl;

    LOG_W << "warning " << 234214.0 << ", " << 123 << " asd";
    LOG_F << "fatal";
    LOG_I << "info";
}
