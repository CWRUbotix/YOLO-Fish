#include <DarkHelp.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>

// DarkHelp CLI doesn't support outputing raw data from videos, so using the API
// https://www.ccoderun.ca/darkhelp/api/Parameters.html

// https://github.com/stephanecharette/DarkHelp/?tab=readme-ov-file#building-darkhelp-linux
// https://www.ccoderun.ca/darkhelp/api/API.html

std::string predictionToMOT(unsigned int frameNumber, const DarkHelp::PredictionResult prediction) {
    // <frame>, <id>, <bb_left>, <bb_top>, <bb_width>, <bb_height>, <conf>, <x>, <y>, <z>
    return std::to_string(frameNumber) + "," +
        //    std::to_string(prediction.object_id) + "," +
        std::to_string(-1) + "," +
        std::to_string(prediction.rect.x) + "," +
        std::to_string(prediction.rect.y) + "," +
        std::to_string(prediction.rect.width) + "," +
        std::to_string(prediction.rect.height) + "," +
        std::to_string(prediction.best_probability) + ",-1,-1,-1";
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::printf("usage: %s <model> <weights> <names> <video>\n", argv[0]);
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

    cv::VideoCapture cap(argv[4]);
    unsigned int frameNumber = 1;

    std::ofstream motOutputFile;
    motOutputFile.open("det.txt", std::ios_base::app);
    while (cap.isOpened())
    {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
        {
            break;
        }

        // https://www.ccoderun.ca/darkhelp/api/structDarkHelp_1_1PredictionResult.html
        const DarkHelp::PredictionResults results = nn.predict(frame);

        // TODO: output in MOT format, then apply SORT (or DeepSORT)
        // https://motchallenge.net/instructions/
        // https://github.com/abewley/sort
        // https://github.com/nwojke/deep_sort

        for (const DarkHelp::PredictionResult &prediction : results) {
            std::cout << predictionToMOT(frameNumber, prediction) << std::endl;
            motOutputFile << predictionToMOT(frameNumber, prediction) << std::endl;
        }

        // TODO: also consider DarkHelp::PositionTracker
        // https://www.ccoderun.ca/darkhelp/api/API.html (see bottom)
        // https://www.ccoderun.ca/darkhelp/api/classDarkHelp_1_1PositionTracker.html


        // std::cout << results << std::endl;

        frameNumber++;
    }
    cap.release();
}