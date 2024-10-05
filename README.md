# ft_ping
```markdown
## Project Description
`ft_ping` is a tool that allows you to ping other devices on the network using the ICMP protocol. This project is used to test network connections and check the accessibility of devices.

## Medium
For more information and articles, visit our Medium page: [fleizean.medium.com](https://fleizean.medium.com/a7abd030df08)

## Installation
To install the project on your local environment, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/fleizean/ft_ping.git
    ```
2. Navigate to the project directory:
    ```sh
    cd ft_ping
    ```
3. Install the necessary dependencies and compile the project:
    ```sh
    make
    ```

## Usage
To run the `ft_ping` tool, use the following command:
    ```sh
    sudo ./ft_ping -h
    ```

## Running Tests
To run the tests for the ft_ping project, use the following command:
    ```sh
    sudo make test
    ```

The tests include:

- `test_send_ping`: Tests the ability to send a ping packet.
- `test_receive_ping`: Tests the ability to receive a ping response.

The test results will be displayed with colored output indicating success or failure.

## Example Output
Here is an example of the test output:
```
Running suite(s): SendReceivePing
Destination IP: 142.250.190.78
Destination Port: 0
bytes_received: 64
sockfd: 3
packet: <packet data>
TEST2: [OK]
All tests passed successfully.
```
If any test fails, the output will indicate the number of failed tests in red.