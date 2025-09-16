#include <DarkHelp.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

// DarkHelp CLI doesn't support outputing raw data from videos, so using the API
// https://www.ccoderun.ca/darkhelp/api/Parameters.html

// https://github.com/stephanecharette/DarkHelp/?tab=readme-ov-file#building-darkhelp-linux
// https://www.ccoderun.ca/darkhelp/api/API.html

std::string predictionToJSON(const DarkHelp::PredictionResult prediction) {
    // <frame>, <id>, <bb_left>, <bb_top>, <bb_width>, <bb_height>, <conf>, <x>, <y>, <z>
    return "        {\n            \"label\": \"" + std::to_string(prediction.best_class) + "\"," +
        "\n            \"x\": \"" + std::to_string(prediction.rect.x) + "\"," +
        "\n            \"y\": \"" + std::to_string(prediction.rect.y) + "\"," +
        "\n            \"width\": \"" + std::to_string(prediction.rect.width) + "\"," +
        "\n            \"height\": \"" + std::to_string(prediction.rect.height) + "\"\n        }";
}

std::string HEADER = "{\n    \"boxes\": [\n";

std::string FOOTER1 = "    ],\n    \"height\": 1080,\n    \"width\": 1920,\n    \"key\": \"";
std::string FOOTER2 = "\"\n}";

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::printf("usage: %s <model> <weights> <names> <dir path>\n", argv[0]);
        return 1;
    }

    std::printf("Model: %s\n", argv[1]);
    std::printf("Weights: %s\n", argv[2]);
    std::printf("Names: %s\n", argv[3]);
    std::printf("Video: %s\n", argv[4]);

    DarkHelp::Config cfg(argv[1], argv[2], argv[3]);
    cfg.threshold = 0.35;
    cfg.include_all_names = false;
    cfg.names_include_percentage = true;
    cfg.annotation_include_duration = true;
    cfg.annotation_include_timestamp = false;
    cfg.sort_predictions = DarkHelp::ESort::kAscending;
    DarkHelp::NN nn(cfg);

    std::ofstream outputFile;
    std::string outputDir = "roboflow/";
    std::vector<fs::directory_entry> entries;
    for (const auto &entry : fs::directory_iterator(argv[4])) {
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end());
    for (const auto &entry : entries)
    {
        cv::Mat img = cv::imread(entry.path().string());
        if (img.empty())
        {
            break;
        }

        // https://www.ccoderun.ca/darkhelp/api/structDarkHelp_1_1PredictionResult.html
        const DarkHelp::PredictionResults results = nn.predict(img);

        // TODO: output in MOT format, then apply SORT (or DeepSORT)
        // https://motchallenge.net/instructions/
        // https://github.com/abewley/sort
        // https://github.com/nwojke/deep_sort

        std::string filename = entry.path().stem().string();
        outputFile.open(outputDir + filename + ".json", std::ios_base::app);
        outputFile.write(HEADER.c_str(), HEADER.length());
        for (long unsigned int i = 0; i < results.size(); i++) {
            std::string json = predictionToJSON(results[i]);
            outputFile.write(json.c_str(), json.length());
            if (i < results.size() - 1) {
                outputFile.write(",", 1);
            }
            outputFile.write("\n", 1);
        }
        outputFile.write(FOOTER1.c_str(), FOOTER1.length());
        outputFile.write(filename.c_str(), filename.length());
        outputFile.write(FOOTER2.c_str(), FOOTER2.length());
        outputFile.close();
    }
}