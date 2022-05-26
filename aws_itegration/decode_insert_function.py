import json
import boto3
import base64

dynamodb = boto3.resource("dynamodb")
lbd = boto3.client('lambda')

#function to get timestamp from the messages
def get_timestamp(event):
    timestamp = event['uplink_message']['rx_metadata'][0]['timestamp']
    return timestamp
    
#function to decode based64 payload 
def decode_payload(event):
    payload_base64 = event["uplink_message"]["frm_payload"]
    payload_dec = base64.b64decode(payload_base64).decode('utf-8')
    print(payload_dec)
    return payload_dec

#fuction to insert soil_moisture values inside table in db   
def insert_soil_table(event):
    table = dynamodb.Table('soil_data')
    decoded_str = decode_payload(event)
    decoded_str = decoded_str[1:len(decoded_str)]
    response = table.put_item(
    Item= {
            'sample_time': get_timestamp(event),
            'device_ID': event['end_device_ids']['device_id'],
            'date': event['received_at'][:-11],
            'soil_moisture': decoded_str
        }
    )  
#fuction to insert temperature values inside table in db
def insert_temperature_table(event):
    table = dynamodb.Table('temperature_data')
    decoded_str = decode_payload(event)
    decoded_str = decoded_str[1:len(decoded_str)]
    response = table.put_item(
    Item= {
            'sample_time': get_timestamp(event),
            'device_ID': event['end_device_ids']['device_id'],
            'date': event['received_at'][:-11],
            'temperature': decoded_str
        }
    )
#fuction to insert irrigation_time values inside table in db
def insert_irrigation_table(event):
    table = dynamodb.Table('irrigation_data')
    decoded_str = decode_payload(event)
    decoded_str = decoded_str[1:len(decoded_str)]
    response = table.put_item(
    Item= {
            'sample_time': get_timestamp(event),
            'device_ID': event['end_device_ids']['device_id'],
            'date': event['received_at'][:-11],
            'irrigation_quantity': decoded_str
        }
    )
  
def lambda_handler(event, context):
    # TODO implement
    # print(event['data']['uplink_message']['frm_payload'])
    message = decode_payload(event)
    if(message[0] == 'T'):
        insert_temperature_table(event)
    elif(message[0] == 'S'):
        insert_soil_table(event)
    elif(message[0] == 'I'):
        insert_irrigation_table(event)
    else :
        print("Message not recognized")
    

    return {
        'statusCode': 200,
        'body': json.dumps('Done')
    }



  
