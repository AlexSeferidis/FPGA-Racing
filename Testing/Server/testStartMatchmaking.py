import requests
import time

def get_auth_token(username, password):
    url = 'https://x3oc3nzf94.execute-api.us-east-1.amazonaws.com/test/login'  
    data = {
        'username': username,
        'password': password
    }
    response = requests.post(url, json=data)
    if response.status_code == 200:
        return response.json()['tokens']['AccessToken']
    else:
        print(f"Failed to obtain auth token. Status code: {response.status_code}")
        return None
    
def test_latency_with_auth_token(url, headers, num_requests):
    latency_times = []
    auth_token = get_auth_token('username', 'Password123!')

    if auth_token:
        headers['Authorization'] = f'Bearer {auth_token}'
        for i in range(num_requests):
            start_time = time.time()
            response = requests.get(url, headers=headers)
            end_time = time.time()
            latency = end_time - start_time
            latency_times.append(latency)
            print(f"Request {i+1}: Latency = {latency} seconds")
        
        average_latency = sum(latency_times) / len(latency_times)
        print(f"\nAverage Latency for {num_requests} requests: {average_latency} seconds")
    else:
        print("Authentication failed. Unable to test latency.")

# Define API endpoint and headers
url = 'https://x3oc3nzf94.execute-api.us-east-1.amazonaws.com/test/startmatchmaking'
headers = {
    'Content-Type': 'application/json'
}

# Number of requests to send
num_requests = 10

# Test latency with authentication token
test_latency_with_auth_token(url, headers, num_requests)