#!/usr/bin/python
#######################################
# This script generates the DFP mappings for LineItem
# Based off: https://github.com/googleads/googleads-python-lib/blob/master/examples/dfp/v201502/line_item_service/get_all_line_items.py
# There are at least 36k+ lineitems, so pulling only the most recent data is recommended. Replace the DAYSAGO variable as desired.
#######################################

# Import appropriate modules from the client library.
from googleads import dfp
import datetime
import json

# REPLACE THIS VARIABLE TO GET DIFFERENT DATE RANGES OF DATA
#DAYSAGO = 3

def main(client):
  #f = open(filename,"a")
  logfile=open('/home/pmccann/logs_daily/log_dfpmaps.log','a')
  # Initialize appropriate service.
  lineitem_service = client.GetService('LineItemService', version='v201502')

  # Create a filter statement.
  timeago = datetime.date.today() - datetime.timedelta(days=daysago)
  values = [{
      'key': 'dateTimeString',
      'value': {
          'xsi_type': 'TextValue',
          'value': timeago.strftime('%Y-%m-%dT%H:%M:%S')
      }
  }]
  query = 'WHERE startDateTime >= :dateTimeString'
  statement = dfp.FilterStatement(query, values)
  #statement = dfp.FilterStatement()

  # Get line items by statement.
  while True:
    logfile.write('lineitems: '+json.dumps(statement.ToStatement())+'\n')
    response = lineitem_service.getLineItemsByStatement(statement.ToStatement())
    if 'results' in response:
      # Display results.
      for lineitem in response['results']:
        print("%s\t%s" % (lineitem['id'], lineitem['name'].encode('utf8').replace('\n', '')))
      statement.offset += dfp.SUGGESTED_PAGE_LIMIT
    else:
      break
  #f.close()

if __name__ == '__main__':
  # Initialize client object.
  dfp_client = dfp.DfpClient.LoadFromStorage('googleads.yaml')
  main(dfp_client)
