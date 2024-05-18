# Project: Convolution and Histograms for Image Editing

This project demonstrates the implementation of convolution operations for image processing in the context of image editors like Photoshop or GIMP. The project uses a convolution kernel to sharpen images and also calculates a grayscale histogram.

## Convolution
Convolution is a technique in image processing where pixel values are computed as weighted sums of surrounding pixels. In this implementation, the following convolution kernel is used for sharpening:

| 0 | -1 | 0 |
|:--------:|:--------:|:--------:|
|  -1  |  5  |  -1  |
|  0  |  -1  |  0  |


Convolution is applied to each pixel in the image, except for the edge pixels, which remain unchanged.

## Histogram
After applying convolution, the project calculates a histogram of the grayscale image. The conversion from RGB to grayscale uses the following formula:

$Y = \text{round}(0.2126 \times R + 0.7152 \times G + 0.0722 \times B)$

The histogram is calculated over five intervals:

1. 0 - 50
2. 51 - 101
3. 102 - 152
4. 153 - 203
5. 204 - 255

## Optimization Technique
* To improve performance and reduce cache-miss counts, the project uses three buffers of appropriate size, which are swapped during processing.
* This allows the program to work primarily with data already loaded in the cache, leading to faster execution and reduced reliance on RAM.


<p align="center">
  <img src="https://hazelcast.com/wp-content/uploads/2021/12/7_CacheMiss_1.png">
</p>


