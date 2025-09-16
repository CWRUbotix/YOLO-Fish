darknet_05_process_videos_multithreaded models/trained_on_merge/yolo-fish-2.cfg fish.names weights/merge_yolo-fish-2.weights videos/short_full.mp4
darknet_05_process_videos_multithreaded models/trained_on_merge/yolov4.cfg fish.names weights/merge_yolov4.weights videos/short_full.mp4

DarkHelp models/trained_on_merge/yolo-fish-2.cfg weights/merge_yolo-fish-2.weights fish.names videos/short_full.mp4
DarkHelp models/trained_on_merge/yolov4.cfg weights/merge_yolov4.weights fish.names videos/short_full.mp4

DarkHelp -j -t 0.01 models/trained_on_merge/yolo-fish-2.cfg weights/merge_yolo-fish-2.weights fish.names /home/mantis/Documents/cwrubotix/data/frames/official00001.png