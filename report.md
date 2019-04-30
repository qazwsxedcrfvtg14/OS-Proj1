OS project1 report
=
## 1. 設計

* 我們使用**clock_gettime**取代**getnstimeofday**，因為可以得到精確度的資訊和一樣準確的效果。
* 我們把要寫入**dmesg**的資料從user space直接寫入"/dev/kmsg"取代呼叫**printk**，因為這樣比較快速安全，避免改到kernel log不必要的地方而造成自己電腦其他問題。
* 使用**sched_setaffinity**來讓使用的CPU都固定在第一顆，這樣能避免一些多Core執行的問題。
* 使用**sched_setscheduler**來調整process的priority，並且設定成FIFO的mode，讓運行中的process不會被搶斷。
* 使用**sched_yield**來把當前process的執行權交給同priority的其他process。
* schedule每隔一段時間會把執行權從process那裡收回來，然後再依照情況分給processes。

## 2. 執行範例測資的結果
https://github.com/qazwsxedcrfvtg14/OS-Proj1/blob/master/dmesg
https://github.com/qazwsxedcrfvtg14/OS-Proj1/tree/master/OS_PJ1_Test
```
FIFO_1
P1 26427
P2 26428
P3 26429
P4 26430
P5 26431

FIFO_2
P1 26436
P2 26437
P3 26438
P4 26439

FIFO_3
P1 26473
P2 26474
P3 26475
P4 26476
P5 26477
P6 26478
P7 26479

FIFO_4
P1 26492
P2 26493
P3 26494
P4 26495

FIFO_5
P1 26500
P2 26501
P3 26502
P4 26503
P5 26504
P6 26505
P7 26506

RR_1
P1 26557
P2 26558
P3 26559
P4 26560
P5 26561

RR_2
P1 26566
P2 26567

RR_3
P1 26577
P2 26578
P3 26579
P4 26580
P5 26581
P6 26582

RR_4
P1 26591
P2 26598
P3 26599
P4 26601
P5 26602
P6 26603
P7 26604

RR_5
P1 26879
P2 26880
P3 26881
P4 26882
P5 26883
P6 26884
P7 26885

SJF_1
P1 26906
P2 26907
P3 26915
P4 26916

SJF_2
P1 26922
P2 26923
P3 26924
P4 26925
P5 26926

SJF_3
P1 26933
P2 26934
P3 26935
P4 26936
P5 26937
P6 26938
P7 26939
P8 26940

SJF_4
P1 26948
P2 26949
P3 26950
P4 26951
P5 26952

SJF_5
P1 26957
P2 26958
P3 26959
P4 26960


PSJF_1
P1 26513
P2 26514
P3 26515
P4 26516

PSJF_2
P1 26522
P2 26523
P3 26524
P4 26525
P5 26526

PSJF_3
P1 26531
P2 26532
P3 26533
P4 26534

PSJF_4
P1 26539
P2 26540
P3 26541
P4 26542

PSJF_5
P1 26547
P2 26548
P3 26549
P4 26550
P5 26551
```

```
FIFO_1
[18742379.859604] [Project1] 26427 1556630002.396618518 1556630003.723154186
[18742381.195962] [Project1] 26428 1556630003.723735910 1556630005.059576070
[18742382.531638] [Project1] 26429 1556630005.060184223 1556630006.395322802
[18742383.865171] [Project1] 26430 1556630006.395879884 1556630007.728916650
[18742385.190484] [Project1] 26431 1556630007.729476411 1556630009.054294586

FIFO_2
[18742597.994019] [Project1] 26436 1556630009.057845834 1556630221.867336732
[18742611.295011] [Project1] 26437 1556630221.868138694 1556630235.168925223
[18742613.952359] [Project1] 26438 1556630235.169686397 1556630237.826421482
[18742616.617525] [Project1] 26439 1556630237.827041699 1556630240.491678526

FIFO_3
[18742637.912021] [Project1] 26473 1556630240.496255720 1556630261.787119227
[18742651.216085] [Project1] 26474 1556630261.787889283 1556630275.091787332
[18742659.197529] [Project1] 26475 1556630275.092534173 1556630283.073596212
[18742661.862928] [Project1] 26476 1556630283.074281649 1556630285.739080440
[18742664.525964] [Project1] 26477 1556630285.740002456 1556630288.402288095
[18742667.180435] [Project1] 26478 1556630288.402864886 1556630291.056864899
[18742677.816566] [Project1] 26479 1556630291.057496512 1556630301.693458132

FIFO_4
[18742683.131067] [Project1] 26492 1556630301.697970202 1556630307.008203702
[18742684.472220] [Project1] 26493 1556630307.008885500 1556630308.349429919
[18742684.997650] [Project1] 26494 1556630308.350006538 1556630308.874894575
[18742686.328842] [Project1] 26495 1556630308.875415962 1556630310.206131254

FIFO_5
[18742707.611935] [Project1] 26500 1556630310.210641227 1556630331.490144390
[18742720.893246] [Project1] 26501 1556630331.490926324 1556630344.772055995
[18742728.875337] [Project1] 26502 1556630344.772749562 1556630352.754504889
[18742731.539991] [Project1] 26503 1556630352.755190654 1556630355.419284172
[18742734.189323] [Project1] 26504 1556630355.419905525 1556630358.068738911
[18742736.848997] [Project1] 26505 1556630358.069345349 1556630360.728510833
[18742747.483799] [Project1] 26506 1556630360.729237480 1556630371.363791202

RR_1
[18742931.904550] [Project1] 26557 1556630554.465789911 1556630555.792719363
[18742933.235613] [Project1] 26558 1556630555.793360244 1556630557.123861892
[18742934.570048] [Project1] 26559 1556630557.124428286 1556630558.458358734
[18742935.893308] [Project1] 26560 1556630558.458912938 1556630559.781677135
[18742937.220598] [Project1] 26561 1556630559.782234917 1556630561.109014119

RR_2
[18742958.776652] [Project1] 26566 1556630562.731121224 1556630582.665999703
[18742962.762985] [Project1] 26567 1556630564.060055065 1556630586.652503375

RR_3
[18743011.244283] [Project1] 26579 1556630597.882343990 1556630635.135948609
[18743015.232698] [Project1] 26577 1556630589.910472589 1556630639.124553674
[18743016.566200] [Project1] 26578 1556630593.897375537 1556630640.458112218
[18743037.831074] [Project1] 26582 1556630608.521037802 1556630661.723915903
[18743043.138533] [Project1] 26581 1556630604.533556837 1556630667.031623704
[18743045.794641] [Project1] 26580 1556630603.206119931 1556630669.687858078

RR_4
[18743060.525735] [Project1] 26601 1556630673.783695732 1556630684.419585569
[18743061.853564] [Project1] 26602 1556630675.112677336 1556630685.747480055
[18743063.193996] [Project1] 26603 1556630676.445419853 1556630687.087958043
[18743084.476706] [Project1] 26599 1556630672.459352620 1556630708.371607078
[18743095.111764] [Project1] 26604 1556630679.096785769 1556630719.007139493
[18743099.104106] [Project1] 26598 1556630671.122894620 1556630722.999648685
[18743107.087053] [Project1] 26591 1556630669.789588676 1556630730.982954768

RR_5
[18743121.789142] [Project1] 26882 1556630734.997460380 1556630745.685677924
[18743123.116079] [Project1] 26883 1556630736.336303738 1556630747.012706458
[18743125.778530] [Project1] 26884 1556630739.008805642 1556630749.675280322
[18743145.795720] [Project1] 26881 1556630733.665075206 1556630769.693326851
[18743156.483829] [Project1] 26885 1556630740.340284886 1556630780.381906199
[18743160.474890] [Project1] 26880 1556630732.337111435 1556630784.373154592
[18743168.461454] [Project1] 26879 1556630730.987819758 1556630792.360066316

SJF_1
[18743174.568566] [Project1] 26907 1556630792.771449170 1556630798.467438416
[18743177.237164] [Project1] 26915 1556630798.468182293 1556630801.136166857
[18743187.877736] [Project1] 26916 1556630801.136801109 1556630811.777199757
[18743206.493976] [Project1] 26906 1556630811.777894084 1556630830.394251392

SJF_2
[18743207.027653] [Project1] 26922 1556630830.664997488 1556630830.927973109
[18743207.565032] [Project1] 26924 1556630830.928924866 1556630831.465387611
[18743218.204267] [Project1] 26923 1556630831.465913537 1556630842.105048590
[18743228.839285] [Project1] 26925 1556630842.105806623 1556630852.740550022
[18743247.453535] [Project1] 26926 1556630852.741268482 1556630871.355616855

SJF_3
[18743255.701408] [Project1] 26933 1556630871.626813944 1556630879.603845987
[18743255.728786] [Project1] 26936 1556630879.604546679 1556630879.631252681
[18743255.756374] [Project1] 26937 1556630879.631807916 1556630879.658845681
[18743266.399984] [Project1] 26938 1556630879.659388801 1556630890.302884915
[18743277.020278] [Project1] 26939 1556630890.303649679 1556630900.923646376
[18743290.313334] [Project1] 26934 1556630900.924405868 1556630914.217292163
[18743308.915611] [Project1] 26935 1556630914.218022187 1556630932.820385570
[18743332.830741] [Project1] 26940 1556630932.821146900 1556630956.736561169

SJF_4
[18743340.805579] [Project1] 26948 1556630956.741204145 1556630964.711771916
[18743343.470176] [Project1] 26949 1556630964.712468397 1556630967.376490690
[18743354.094652] [Project1] 26950 1556630967.377142046 1556630978.001431336
[18743356.753137] [Project1] 26952 1556630978.002121219 1556630980.660043949
[18743362.066915] [Project1] 26951 1556630980.660638026 1556630985.974049380

SJF_5
[18743367.400268] [Project1] 26957 1556630985.987253172 1556630991.307633614
[18743368.725426] [Project1] 26958 1556630991.308298864 1556630992.632868832
[18743370.051611] [Project1] 26959 1556630992.633417312 1556630993.959117288
[18743371.386664] [Project1] 26960 1556630993.959652649 1556630995.294226157

PSJF_1
[18742763.436607] [Project1] 26516 1556630379.342911996 1556630387.317295698
[18742774.060566] [Project1] 26515 1556630376.681699367 1556630397.941741920
[18742789.997267] [Project1] 26514 1556630374.018205680 1556630413.879147114
[18742813.903266] [Project1] 26513 1556630371.367537893 1556630437.786203200

PSJF_2
[18742819.222151] [Project1] 26523 1556630440.451333611 1556630443.105343067
[18742824.544445] [Project1] 26522 1556630437.790704518 1556630448.427869024
[18742832.521111] [Project1] 26525 1556630451.081544168 1556630456.404888501
[18742835.172963] [Project1] 26526 1556630456.405614436 1556630459.056865043
[18742843.138493] [Project1] 26524 1556630448.428517291 1556630467.022732191

PSJF_3
[18742845.800265] [Project1] 26532 1556630468.362154349 1556630469.684642737
[18742847.126403] [Project1] 26533 1556630469.685311381 1556630471.010830862
[18742848.464493] [Project1] 26534 1556630471.011540047 1556630472.348991244
[18742852.443088] [Project1] 26531 1556630467.027304426 1556630476.327747803

PSJF_4
[18742855.372385] [Project1] 26541 1556630476.597126862 1556630479.257180359
[18742860.419286] [Project1] 26540 1556630476.332268521 1556630484.304286116
[18742871.032870] [Project1] 26542 1556630484.305038640 1556630494.918346252
[18742889.630113] [Project1] 26539 1556630494.919034956 1556630513.516403557

PSJF_5
[18742890.165963] [Project1] 26547 1556630513.788178937 1556630514.052302984
[18742890.702657] [Project1] 26549 1556630514.053244708 1556630514.589022288
[18742901.333706] [Project1] 26548 1556630514.589583710 1556630525.220520782
[18742911.959026] [Project1] 26550 1556630525.221296933 1556630535.846313802
[18742930.555902] [Project1] 26551 1556630535.847001384 1556630554.444012769
```


## 3. 比較實際結果與理論結果，並解釋造成差異的原因

* 每組測資第一個圖表為實際時間，第二個圖表為理論時間

1. FIFO
![](https://i.imgur.com/rDmFNNy.png)

2. RR
![](https://i.imgur.com/cPHcL0i.png)
![](https://i.imgur.com/VdAKpCD.png)
![](https://i.imgur.com/AeGextl.png)
![](https://i.imgur.com/3210Zmv.png)

3. SJF
![](https://i.imgur.com/o9uvYK5.png)
![](https://i.imgur.com/5YEqJsw.png)
![](https://i.imgur.com/UhuYO5y.png)
![](https://i.imgur.com/Um1mFk7.png)

4. PSJF
![](https://i.imgur.com/HKz4CN9.png)
![](https://i.imgur.com/ejT5MhC.png)
![](https://i.imgur.com/l0VsrgT.png)
![](https://i.imgur.com/8ySRK7A.png)


* 我們使用了關掉了turbo boost的伺服器來跑我們的程式以減少誤差
* 在程式中使用迴圈來等待執行時間會有迴圈本身每次執行時間不固定的問題
* 因為實驗數據和理論數據太過接近，如果要確認真實性可以檢查我們的數據表格：
> https://docs.google.com/spreadsheets/d/10juCjTlHYU_fRDRzz_UOGjr-l86Pfyujyi1fBLcOJeY/edit

## 4. 各組員的貢獻

B05902086 周　逸：全部的程式部分
B05902052 劉家維：計算理論值、繪製分析圖
B06501051 陳政瑞：寫report、繪製分析圖、協助debug