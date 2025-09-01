# CycleGAN (VanGogh) Compression
This project investigates the practical application of model compression on a large scale Generative Adversarial Network (GAN) for real time deployment on a resource constrained edge device, the Raspberry Pi 5.

The core of this research involved taking a pre-trained, high fidelity CycleGAN model, designed to transform photographs into the artistic style of Vincent van Gogh, and applying state of the art compression techniques to make it efficient enough for live video processing.

The project followed a structured research and engineering workflow.
## Teacher Model

The baseline "teacher" model was the official pretrained style_vangogh CycleGAN from the pytorch-CycleGAN-and-pix2pix repository. This is a ResNet based generator with over 11 million parameters.

## Compression Framework

The compression was performed using the official implementation of the paper "GAN Compression: Efficient Architectures for Interactive Conditional GANs" (Li et al., CVPR 2020). The "Fast GAN Compression" pipeline was chosen for its efficiency. This involves two key stages:

1. Super Network Training (train_supernet.py): A large "once-for-all" network was trained for 120 epochs using the pretrained Van Gogh model as a teacher. This super network contains a vast search space of potential compressed architectures.

2. Evolutionary Search (evolution_search.py): An evolutionary algorithm was used to search through the trained super network to find the optimal subnetwork (the "student" model) that best balanced performance (measured by FID) and a given computational budget (MACs).

## Experimental Setup

To analyze the performance trade offs, this process was repeated for three distinct compression targets:

1. Mild Compression (Target: 2.0G MACs)
2. Moderate Compression (Target: 1.75G MACs)
3. Aggressive Compression (Target: 1.5G MACs)


## Results
The compression was highly effective, significantly reducing the model's size while preserving the core artistic style.

| Compression Level | MACs | #Parameters |
| ------------- | ------------- | ------------- |
| Baseline  | 11.3G | |
| Mild | 2.0G | ~54.7x |
| Moderate | 1.75G | ~72.4x |
| Aggressive | 1.5G | ~74.1x |




## Visual Interpretation
This repository also contains modules for interpretation on an RTSP streaming device (like a smartphone) or OV7670 (no FIFO) camera connected to an STM32F7 or ESP32S3.




## Acknowledgements

This work is built upon the foundational research and code from two incredible projects:

[pytorch-CycleGAN-and-pix2pix](https://github.com/mit-han-lab/gan-compression?tab=readme-ov-file) by Zhu et al.

[gan-compression](https://github.com/mit-han-lab/gan-compression) by Li, Lin, Han et al.
