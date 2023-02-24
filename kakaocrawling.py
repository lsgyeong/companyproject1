import requests
import pymysql
import csv

##카카오 API
def whole_region(keyword, start_x,start_y,end_x,end_y):
    #print(start_x,start_y,end_x,end_y)
    page_num = 1
    # 데이터가 담길 리스트
    all_data_list = []

    while (1):
        url = 'https://dapi.kakao.com/v2/local/search/keyword.json'
        params = {'query': keyword, 'page': page_num,
                  'rect': f'{start_x},{start_y},{end_x},{end_y}'}
        headers = {"Authorization": "KakaoAK 67ce0f01ed0a5ab169be0758cad914cb"}
        ## 입력예시 -->> headers = {"Authorization": "KakaoAK f64acbasdfasdfasf70e4f52f737760657"}
        resp = requests.get(url, params=params, headers=headers)

        search_count = resp.json()['meta']['total_count']
        # print('총 개수', search_count)

        if search_count > 45:
            print('좌표 4등분')
            dividing_x = (start_x + end_x) / 2
            dividing_y = (start_y + end_y) / 2
            ## 4등분 중 왼쪽 아래
            all_data_list.extend(whole_region(keyword, start_x, start_y, dividing_x, dividing_y))
            ## 4등분 중 오른쪽 아래
            all_data_list.extend(whole_region(keyword, dividing_x, start_y, end_x, dividing_y))
            ## 4등분 중 왼쪽 위
            all_data_list.extend(whole_region(keyword, start_x, dividing_y, dividing_x, end_y))
            ## 4등분 중 오른쪽 위
            all_data_list.extend(whole_region(keyword, dividing_x, dividing_y, end_x, end_y))
            return all_data_list

        else:
            if resp.json()['meta']['is_end']:
                all_data_list.extend(resp.json()['documents'])
                return all_data_list
            # 아니면 다음 페이지로 넘어가서 데이터 저장
            else:
                print('다음페이지')
                page_num += 1
                all_data_list.extend(resp.json()['documents'])

def overlapped_data(keyword, start_x, start_y, next_x, next_y, num_x, num_y):
    # 최종 데이터가 담길 리스트
    overlapped_result = []

    # 지도를 사각형으로 나누면서 데이터 받아옴
    for i in range(1, num_x + 1):  ## 1,10
        end_x = start_x + next_x
        initial_start_y = start_y
        for j in range(1, num_y + 1):  ## 1,6
            end_y = initial_start_y + next_y
            each_result = whole_region(keyword, start_x, initial_start_y, end_x, end_y)
            overlapped_result.extend(each_result)
            initial_start_y = end_y
        start_x = end_x
    return overlapped_result


conn = pymysql.connect(host='127.0.0.1', port=3306, user='root', password='1234', db='company1')
c = conn.cursor()
c.execute(f"select store,longitude,latitude from company1.starbucks")
temp_db = c.fetchall()
conn.close()

result=[]
for i in range(len(temp_db)):
    print(f'-----------------{temp_db[i][0]}--------------------')

    start_x= temp_db[i][1]-0.004480
    start_y= temp_db[i][2]-0.005445
    next_x=0.001    # 좌표 이동
    next_y=0.001    # 좌표 이동
    num_x=11        # 움직일 경도
    num_y=11        # 움직일 위도

    a_list=['학교']
    b_list=[]
    for j in a_list:
        overlapped_result = overlapped_data(j, start_x, start_y, next_x, next_y, num_x, num_y)
        b_list.append(overlapped_result)

    for k in b_list[0]:
        if k['category_group_name'] == '학교':
            print(temp_db[i][0],k['place_name'],k['road_address_name'])
            temp=[temp_db[i][0],k['place_name'],k['road_address_name']]
            result.append(temp)
print(result)

# csv파일에 넣기
f = open('star_education.csv', 'w', newline='')
wr = csv.writer(f)
wr.writerows(result)
f.close()







