# wine-prediction

# Setting up EC2 Cluster on AWS

To execute the ML container application for training across multiple parallel EC2 instances, follow these steps to create a cluster comprising 4 instances:

1. Begin in the AWS Management Console and search for Elastic Container Service (ECS), then click on it. Within the ECS Console, select "Cluster" and click on "Create Cluster."

2. In the "Select cluster template," opt for "EC2 Linux + Networking" since the Amazon Linux 2 image will be utilized for managing ECS tasks.

3. In the "Configure cluster," adjust the following parameters while leaving the rest unchanged:

   - Cluster name: wine-quality-train-cluster
   - Provisioning model: On-Demand Instance
   - EC2 instance type: t2.micro
   - Number of instances: 4
   - Key Pair: Select an appropriate key pair
   - Security group inbound rules (Port range): 22-80

4. Click on "Create" to initiate the cluster creation process.

Once the cluster is successfully created, you'll notice that 4 EC2 instances are registered to the cluster. You can verify this by checking the EC2 dashboard and confirming the number of running EC2 instances.

# Setting up Task Definitions and Tasks

After successfully setting up a cluster, the next step is to initiate the execution of our ML container application by creating a "Task Definition." This definition provides essential information about containers, including bind mounts, volumes, and other configurations. Here's a detailed breakdown of the process:

1. In the ECS console, navigate to "Task Definitions" and select "Create New Task Definition."

2. Choose "EC2" as the launch type compatibility, opening the "Configure Task and Container Definitions" screen to set up the necessary parameters for the Docker container.

3. For the ML container application used in training, which generates two critical files (Modelfile and results.txt), a bind-mount between the Docker container and the host is required for file accessibility. Configure as follows:

   - Task Definition Name: wine-quality-train-task
   - Task Role: ecsTaskExecutionRole
   - Under "Volumes," click on "Add volume":
     - Name: host-path
     - Volume type: Bind Mount
     - Source path: /home/ec2-user

4. Next, configure the ML container for training under "Container Definitions." Click on "Add container":

   - Container Name: wine-prediction-app
   - Image: rasagnakamineni/wine-prediction-app:ec2
   - Memory Limits: *Soft Limit, 512
   - Under "Mount points":
     - Source volume: host-path
     - Container path: /job

5. Click on "Add" and then "Create" to finalize the Task Definition.

# Running the Task

To run the task and initiate the Docker container application on EC2 instances:

1. On the ECS console, select "Cluster" and choose the recently created cluster (wine-train-quality-cluster).

2. Go to the "Tasks" tab and click on "Run new Task."

3. Select the launch type as "EC2" and set the number of tasks to "4."

4. Click on "Run Task" to commence the execution process.

# Launching an Instance on AWS

To launch an Ubuntu instance on AWS, follow these steps:

1. Go to the EC2 dashboard and click on "Launch instances."

2. In "Choose an Amazon Machine Image (AMI)," select "Amazon Linux 2 AMI (HVM), SSD Volume Type."

3. In "Choose an Instance type," select "t2.micro" and click on "Review and Launch."

4. Click on "Launch."

5. Create a New key pair or choose an existing one and click on "Launch."

# Running Prediction App Using EC2 Docker Instance

To execute the prediction app using an EC2 instance with Docker, follow these steps:

1. Navigate to your EC2 instances, locate the instance named "wine-prediction-app."

2. Connect to this instance via EC2 Instance Connect.

3. Run the following commands in the terminal:

    ```bash
    sudo docker pull rasagnakamineni/wine-prediction-app:ec2
    sudo docker run --rm -it rasagnakamineni/wine-prediction-app:ec2
    ```

# Running Training on Spark Cluster in Amazon EMR

To perform training on a Spark cluster in Amazon EMR, follow these steps:

1. Access Amazon EMR and select the latest cluster available.

2. Clone the selected cluster. Note that it might take approximately 30 minutes for the clone to start and run.

3. Proceed with your training tasks on the cloned Spark cluster.

# Running Without Docker

To run the Wine Quality Prediction App without Docker, follow these steps:

1. Run the following Maven command to clean and package the application into a JAR file:

    ```bash
    mvn clean package
    ```

2. Once the JAR file is packaged, you have two options:

    a. If the JAR file is available in a bucket accessible to you, download it from the bucket.

    b. Or if you've already packaged it locally, use the generated JAR file.

3. Use the following command with `spark-submit` to execute the Wine Quality Prediction App:

    ```bash
    spark-submit --class com.example.WineQualityPredictionApp --master local --deploy-mode client winepredictionapp-1.0-SNAPSHOT.jar <S3_DATASET_URL>
    ```

Replace `<S3_DATASET_URL>` with the actual URL pointing to your dataset stored in an S3 bucket.

This command runs the application utilizing Spark with the specified class and local master, deploying it in client mode.


