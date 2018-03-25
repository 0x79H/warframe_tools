#  coding=utf-8
import ctypes
import json
import urllib2
from collections import OrderedDict
from bs4 import BeautifulSoup
import time

wanna_item = []
alert = True
#alert = False

def get_price(items):
    temp = []
    res = urllib2.urlopen("https://api.warframe.market/v1/items/%s/orders" % items["web_name"])
    js = json.load(res)
    out = sorted(js['payload']['orders'], key=lambda e: e['platinum'], reverse=True)
    for i in out:
        if (i[u'order_type'] == u'sell') & (i[u'user'][u'status'] == u'ingame'):
            # print (u"卖家 %+30s 的报价为 %d" % (i[u"user"][u"ingame_name"], i[u"platinum"]))
            temp.append(i)
            # output = i
    # return output
    for i in range(-5, 0, 1):
        print (u"卖家 %+30s 的报价为 %d" % (temp[i][u"user"][u"ingame_name"], temp[i][u"platinum"]))
    return temp[-1]


# copy frome pyperclip
def winSetClipboard(text):
    GMEM_DDESHARE = 0x2000
    ctypes.windll.user32.OpenClipboard(0)
    ctypes.windll.user32.EmptyClipboard()
    try:
        # works on Python 2 (bytes() only takes one argument)
        hCd = ctypes.windll.kernel32.GlobalAlloc(GMEM_DDESHARE, len(bytes(text)) + 1)
    except TypeError:
        # works on Python 3 (bytes() requires an encoding)
        hCd = ctypes.windll.kernel32.GlobalAlloc(GMEM_DDESHARE, len(bytes(text, 'ascii')) + 1)
    pchData = ctypes.windll.kernel32.GlobalLock(hCd)
    try:
        # works on Python 2 (bytes() only takes one argument)
        ctypes.cdll.msvcrt.strcpy(ctypes.c_char_p(pchData), bytes(text))
    except TypeError:
        # works on Python 3 (bytes() requires an encoding)
        ctypes.cdll.msvcrt.strcpy(ctypes.c_char_p(pchData), bytes(text, 'ascii'))
    ctypes.windll.kernel32.GlobalUnlock(hCd)
    ctypes.windll.user32.SetClipboardData(1, hCd)
    ctypes.windll.user32.CloseClipboard()


def find_good_price(item):
#    print(u"[%s]===============================================" % item["cn_name"])
    print("="*10+("%s"%time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))+"="*10)
    try:
        min_order = get_price(item)
    except  Exception:
        print(u"%s查询出错"%"["+item["cn_name"]+"]")
        time.sleep(60)
        return
#    print(u"%s \r\n查询的[%10s]最低价为: %10d" % (time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),item["cn_name"], min_order[u'platinum']))
    print(u"查询的%10s最低价为: %10d" % ("["+item["cn_name"]+"]", min_order[u'platinum']))
    chat = u"/w %s Hi! I want to buy: %s for %d platinum. (warframe.market)" % (
        min_order[u'user'][u'ingame_name'], item["name"], min_order[u'platinum'])
    print(chat)
    if ((alert==True)&(min_order[u'platinum'] < item["min_price"]) &(min_order[u'platinum'] <item["wanna_price"])):
        ctypes.windll.user32.MessageBoxW(0, u"[%s]当前的最低价格为%d" % (item["cn_name"], min_order[u'platinum']), u"最低价!", 0x00001040L)
        winSetClipboard(chat)
    if(item["min_price"]<min_order[u'platinum']):
        with open("D:\\warframe.txt","a") as f:
            f.write((u"%s%20s                  低价%10d\r"%(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),item["cn_name"],min_order[u'platinum'])).encode('utf-8'))
    item["min_price"] = min_order[u'platinum']
    time.sleep(30)


def get_name():
    res2 = urllib2.urlopen(
        "https://warframe.huijiwiki.com/wiki/warframe%E4%B8%AD%E6%96%87%E7%BB%B4%E5%9F%BA:%E4%B8%AD%E8%8B%B1%E5%90%8D%E7%A7%B0%E5%AF%B9%E7%85%A7")

    table_data = [[cell.text for cell in row("td")]
                  for row in BeautifulSoup(res2)("tr")]
    jsjs = json.load(OrderedDict(table_data))
    pass


def add_item(name, cn_name, web_name, price=0xFFFFFFFF):
    wanna_item.append({"name": name, "cn_name": cn_name.decode('utf-8'), "web_name": web_name, "min_price": 0xFFFFFFFF,"wanna_price":price})
#def add_item(name, cn_name, web_name):
#    wanna_item.append({"name": name, "cn_name": cn_name.decode('utf-8'), "web_name": web_name, "min_price": 0xFFFFFFFF,"wanna_price":0xFFFFFFFF})


def set_wanna_dict():
# 【锯刃】【漂移接触】 切割
    add_item("Maiming Strike", "致残突击", "maiming_strike",400) #370 #390
    add_item("Primed Pressure Point","压迫点 Prime","primed_pressure_point",90)
    add_item("VOLTAIC STRIKE", "伏打电能", "voltaic_strike",90)
#    add_item("Buzz Kill", "败兴虐杀", "buzz_kill") #14
    add_item("Secura Lecta", "保障 勒克塔", "secura_lecta",50)
#    add_item("Drifting Contact", "漂移接触", "drifting_contact",20) #17
#    add_item("Virulent Scourge", "剧毒灾害", "virulent_scourge") #5p
#    add_item("Primed Reach","剑风 Prime","primed_reach",90)#80
#    add_item("Jagged Edge","锯刃","jagged_edge",4) #3
    pass


if __name__ == '__main__':
    set_wanna_dict()
    while True:
        for item in wanna_item:
            find_good_price(item)
            
            
            
            
            
            
            
            
    # wanna_item["maiming_strike"]=u"致残突击"
#    get_name()
# 【败兴虐杀】【锯刃】【漂移接触】
# find_good_price("maiming_strike")
# Maiming Strike maiming_strike
#    find_good_price("voltaic_strike") #伏打电能 100p // 03-19 90
#    find_good_price("buzz_kill") #Buzz Kill 19p [败兴虐杀]  0318--15p
#    find_good_price("secura_lecta") #Secura Lecta 49p// [保障 勒克塔]
#    find_good_price("drifting_contact")#Drifting Contact 20p //[漂移接触] 
#    find_good_price("virulent_scourge")#Virulent Scourge 5p  [剧毒灾害] 0318--4p
# Berserker 17p      14收到

#    [急进猛突] [狂战士] [漂移接触] [压迫点] [压迫点 Prime] [剑风] [剑风 Prime] [狂暴] [败兴虐杀] [炙热黄蜂] [阿特拉克斯]
# [异况超量] [剧毒灾害]
