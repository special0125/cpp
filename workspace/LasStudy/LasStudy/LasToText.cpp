#include <liblas/liblas.hpp>    // liblas ���̺귯�� ���
#include <fstream>
#include <iostream>
#include <limits>


int main() {
    // ���� ����
    int i = 1;
    std::string lasfile = "C:/Users/516/Desktop/������/cppStudy/ChunAn_Error_1.las";   // .las ���� ��ġ
    std::string txtfile = "C:/Users/516/Desktop/������/cppStudy/ChunAn_Error_1.txt";   // ������ text ����

    // ����� ��Ʈ�� ����
    std::ifstream ifs(lasfile, std::ios::in | std::ios::binary);    // �б� ���, ���̳ʸ����Ϸ� ����(���� ����)
    std::ofstream ofs(txtfile);

    // ������ ������ ���� ��� ����
    if (!ifs.is_open()) {
        std::cerr << "Error: Unable to open LAS file: " << lasfile << std::endl;
        return 1;
    }

    // liblas ����, ��� ����
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const& header = reader.GetHeader();
    
    // ��� ���� ���
    ofs << "Header" << std::endl;
    ofs << "Compressed: " << ((header.Compressed() == true) ? "true" : "flase") << std::endl;
    ofs << "Signature: " << header.GetFileSignature() << std::endl;
    ofs << "Points count: " << header.GetPointRecordsCount() << std::endl;
    ofs << "Version: " << header.GetVersionMajor() << std::endl;
    ofs << "Creation Date: " << header.GetCreationYear() << std::endl;
    ofs << "Project ID: " << header.GetProjectId() << std::endl;
    ofs << "System ID: " << header.GetSystemId() << std::endl;
    ofs << "Software ID: " << header.GetSoftwareId() << std::endl;
    ofs << "Offset: " << header.GetDataOffset() << std::endl;
    ofs << "Scale Factor X: " << header.GetScaleX() << std::endl;
    ofs << "Scale Factor Y: " << header.GetScaleY() << std::endl;
    ofs << "Scale Factor Z: " << header.GetScaleZ() << std::endl << std::endl;

    // VLR ���� ���
    const std::vector<liblas::VariableRecord>& vlrs = header.GetVLRs();
    for (const auto&vlr : vlrs) {
        try {
            liblas::VariableRecord variableRecord;
            ofs << "variable length header record " << i << " of 3:" << std::endl;
            ofs << "Reserved: " << vlr.GetReserved() << std::endl;
            ofs << "User ID: " << vlr.GetUserId(1) << std::endl;
            ofs << "Record ID: " << vlr.GetRecordId() << std::endl;
            ofs << "Length after header: " << vlr.GetRecordLength() << std::endl;
            ofs << "description: " << vlr.GetDescription(1) << std::endl << std::endl;
            i++;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }
        
    }

    // ����Ʈ �ּڰ�, �ִ� ���
    ofs << "reporting minimum and maximum for all LAS point record entries ..." << std::endl;
    ofs << "X Min: " << header.GetMinX() << " Max: " << header.GetMaxX() << std::endl;
    ofs << "Y Min: " << header.GetMinY() << " Max: " << header.GetMaxY() << std::endl;
    ofs << "Z Min: " << header.GetMinZ() << " Max: " << header.GetMaxZ() << std::endl << std::endl;
    
    // ����Ʈ ������ �ּڰ�, �ִ밪 ���� ���
    int minIntensity = std::numeric_limits<int>::max();
    int maxIntensity = std::numeric_limits<int>::lowest();
    int minReturnNumber = std::numeric_limits<int>::max();
    int maxReturnNumber = std::numeric_limits<int>::lowest();
    int minNumberOfReturns = std::numeric_limits<int>::max();
    int maxNumberOfReturns = std::numeric_limits<int>::lowest();
    int minEdgeOfFlightLine = std::numeric_limits<int>::max();
    int maxEdgeOfFlightLine = std::numeric_limits<int>::lowest();
    int minScanDirectionFlag = std::numeric_limits<int>::max();
    int maxScanDirectionFlag = std::numeric_limits<int>::lowest();
    int minScanAngleRank = std::numeric_limits<int>::max();
    int maxScanAngleRank = std::numeric_limits<int>::lowest();
    int minUserData = std::numeric_limits<int>::max();
    int maxUserData = std::numeric_limits<int>::lowest();
    int minPointSourceID = std::numeric_limits<int>::max();
    int maxPointSourceID = std::numeric_limits<int>::lowest();
    double minGpsTime = std::numeric_limits<double>::max();
    double maxGpsTime = std::numeric_limits<double>::lowest();
    while (reader.ReadNextPoint()) {
        liblas::Point const& p = reader.GetPoint();
        int intensity = p.GetIntensity();
        int returnNumber = p.GetReturnNumber();
        int numberOfReturns = p.GetNumberOfReturns();
        int edgeOfFlightLine = p.GetFlightLineEdge();
        int scanDirectionFlag = p.GetScanFlags();
        int scanAngleRank = p.GetScanAngleRank();
        int userData = p.GetUserData();
        int pointSourceID = p.GetPointSourceID();
        double gpsTime = p.GetTime();

        minIntensity = std::min(minIntensity, intensity);
        maxIntensity = std::max(maxIntensity, intensity);
        minReturnNumber = std::min(minReturnNumber, returnNumber);
        maxReturnNumber = std::max(maxReturnNumber, returnNumber);
        minNumberOfReturns = std::min(minNumberOfReturns, numberOfReturns);
        maxNumberOfReturns = std::max(maxNumberOfReturns, numberOfReturns);
        minEdgeOfFlightLine = std::min(minEdgeOfFlightLine, edgeOfFlightLine);
        maxEdgeOfFlightLine = std::max(maxEdgeOfFlightLine, edgeOfFlightLine);
        minScanDirectionFlag = std::min(minScanDirectionFlag, scanDirectionFlag);
        maxScanDirectionFlag = std::max(maxScanDirectionFlag, scanDirectionFlag);
        minScanAngleRank = std::min(minScanAngleRank, scanAngleRank);
        maxScanAngleRank = std::max(maxScanAngleRank, scanAngleRank);
        minUserData = std::min(minUserData, userData);
        maxUserData = std::max(maxUserData, userData);
        minPointSourceID = std::min(minPointSourceID, pointSourceID);
        maxPointSourceID = std::max(maxPointSourceID, pointSourceID);
        minGpsTime = std::min(minGpsTime, gpsTime);
        maxGpsTime = std::max(maxGpsTime, gpsTime);

    }
    // �ּڰ��� �ִ� ���
    ofs << "Intensity: " << minIntensity << "\t" << maxIntensity << std::endl;
    /*ofs << "Return Number: " << minReturnNumber << "\t" << maxReturnNumber << std::endl;
    ofs << "Number Of Returns: " << minNumberOfReturns << "\t" << maxNumberOfReturns << std::endl;
    ofs << "Edge Of Flight Line: " << minEdgeOfFlightLine << "\t" << maxEdgeOfFlightLine << std::endl;
    ofs << "Scan Direction Flag: " << minScanDirectionFlag << "\t" << maxScanDirectionFlag << std::endl;
    ofs << "Scan Angle Rank: " << minScanAngleRank << "\t" << maxScanAngleRank << std::endl;
    ofs << "User Data: " << minUserData << "\t" << maxUserData << std::endl;
    ofs << "Point Source ID: " << minPointSourceID << "\t" << maxPointSourceID << std::endl;
    ofs << "GPS Time: " << minGpsTime << "\t" << maxGpsTime << std::endl;*/

    std::cout << "LAS to TXT conversion completed successfully." << std::endl;
    ifs.close();
    return 0;

}


