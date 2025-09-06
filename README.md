# CycleGAN (VanGogh) Compression
This project investigates the practical application of model compression on a large scale Generative Adversarial Network (GAN) for real time deployment on a resource constrained edge device, the Raspberry Pi 5. This repository contains additionally OV7670 modules for STM32F7 and ESP32S3, with an RTSP receiver module for added smartphone connectivity. These visual interpretation modules, the gan compression and an additional temperature sensor communication demo were created at a summer internship at MS SPEKTRAL.

The core of this research involved taking a pre-trained, high fidelity CycleGAN model, designed to transform photographs into the artistic style of Vincent van Gogh, and applying state of the art compression techniques to make it efficient enough for live video processing.

<img width="976" height="506" alt="Untitled" src="https://github.com/user-attachments/assets/fea33b80-105b-4de4-b942-f04bf4b37fe5" />
<img width="976" height="506" alt="Untitled" src="https://github.com/user-attachments/assets/b1779f18-2076-46e2-9592-2e763e548998" />
<img width="976" height="506" alt="Untitled" src="https://github.com/user-attachments/assets/a39f03f7-c968-4f92-ad04-28f12354e538" />
<img width="976" height="506" alt="Untitled" src="https://github.com/user-attachments/assets/3947ae97-4f9f-4d7c-ba96-7ed68e9220e0" />


<br>
<br>

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

<br>
<br>

## Results
The compression was highly effective, significantly reducing the model's size while preserving the core artistic style.

| Compression Level | MACs | #Parameters |
| ------------- | ------------- | ------------- |
| Baseline  | 11.3G | - |
| Mild | 2.0G | ~54.7x |
| Moderate | 1.75G | ~72.4x |
| Aggressive | 1.5G | ~74.1x |

<br>
<br>


## Visual Interpretation
This repository also contains modules for interpretation on an RTSP streaming device (like a smartphone) or OV7670 (no FIFO) camera connected to an STM32F7 or ESP32S3.

<br>
<br>

## Acknowledgements

This work is built upon the foundational research and code from two incredible projects:

[pytorch-CycleGAN-and-pix2pix](https://github.com/mit-han-lab/gan-compression?tab=readme-ov-file) by Zhu et al.

[gan-compression](https://github.com/mit-han-lab/gan-compression) by Li, Lin, Han et al.
