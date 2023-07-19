import sensor, image, time
from micropython import const
from collections import deque
from machine import UART
from fpioa_manager import fm
from Maix import GPIO

"""
==============================================================
閾値：定数
    - GOAL_THRESH：青、黄色ゴールのLAB閾値
    - LINE_THRESH：白線のLAB閾値
    - FIELD_THRESH：緑フィールドのLAB閾値
==============================================================
"""

GOAL_THRESH = [
#(0, 100, -128, 127, -128, -27) #青色
(0, 100, -39, 0, 76, 127) #黄色
]

LINE_THRESH = [
(0, 6, -128, 127, -128, 2)
]

FIELD_THRESH = [
(0, 100, -128, -16, -128, 127)
]

"""
==============================================================
Blob：クラス
    - 領域検出に関するクラス
    - 面積とImage.blobクラスを持つ
    - 矩形の描画、カメラの視野角から角度の算出を行う
==============================================================
"""

class Blob:
    def __init__(self, area, blob):
        self._area = area
        self._blob = blob

    def set_blob(self, area, blob):
        self._area = area
        self._blob = blob

    def draw_rect(self, img, color):
        img.draw_rectangle(self._blob.x(), self._blob.y(), self._blob.w(), self._blob.h(), color)
        img.draw_cross(self._blob.cx(), self._blob.cy(), (0, 0, 0))

    def culc_xangle(self):
        return int((self._blob.cx() / 320) * 110)

"""
==============================================================
Line：クラス
    - ライン検出に関するクラス
    - 線形回帰結果ととImage.lineクラスを持つ
    - 線の描画を行う
==============================================================
"""

class Line:
    def __init__(self, regression):
        self._regression = regression
        self._line = regression.line()

    def set_regression(self, regression):
        self._regression = regression
        self._line = regression.line()

    def draw_line(self, img, thickness):
        if self._regression:
            img.draw_line(self._line, thickness=thickness)

"""
==============================================================
get_blob：メソッド
    - 探索画像と閾値を引数に持ち閾値の領域を検出し面積が最大のものを返す
    - Blob型の変数maxblob(初期値面積0)を作成し、面積をもとに大きいものに更新していく
==============================================================
"""

def get_blob(img, thresh):
    maxblob = Blob(0,None)

    for blob in img.find_blobs(thresh, pixel_threshold=100, area_threshold=100, merge = True, margin = 5):
        if maxblob._area < blob.area():
            maxblob.set_blob(blob.area(), blob)

    return maxblob


if __name__ == "__main__":
    #カメラのセットアップ
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_saturation(3)
    sensor.set_contrast(3)
    sensor.set_brightness(1)
    sensor.set_vflip(1)
    sensor.skip_frames(time = 100)

    clock = time.clock()

    #UARTのセットアップ TX=35番, RX=34番
    fm.register(34, fm.fpioa.UART1_TX, force=True)
    fm.register(35, fm.fpioa.UART1_RX, force=True)
    uart = UART(UART.UART1, 115200, timeout_char = 100)

    angle_list = []


    while(True):
        clock.tick()

        img = sensor.snapshot()

        """
        ==============================================================
        白線検知
            - フィールド内の緑の領域をくり抜いてから線形回帰で白線を探す
            - 検出されるラインの座標はくり抜いた画像内での座標なので元の座標に変換
        ==============================================================
        """

        #field = get_blob(img, FIELD_THRESH)

        #if field._blob:
            #field.draw_rect(img, color = (0, 255, 0))
            #field_img = img.copy((field._blob.x(), field._blob.y(), field._blob.w(), field._blob.h()))

            #reg = field_img.get_regression(LINE_THRESH)

            #if reg:
                #line = Line(reg)
                #img.draw_line(field._blob.x() + line._line[0], field._blob.y() + line._line[2],
                                #field._blob.x() + line._line[1], field._blob.y() + line._line[3], thickness = 4)



        """
        ==============================================================
        ゴール検知
            - ゴールの色から領域を検出
            - カメラの視野角から実際の角度を算出
        ==============================================================
        """

        goal = get_blob(img, GOAL_THRESH)

        if goal._blob:
            x_angle = goal.culc_xangle()
            #if len(queue) <= 10:
                #queue.insert(0,x_angle)
            #else:
                #queue.pop()
                #queue.insert(0,x_angle)
            #x_ave = 0
            #for val in queue:
                #x_ave += val / len(queue)
            if len(angle_list) < 10:
                angle_list.append(x_angle)
            else:
                angle_list.pop(0)
                angle_list.append(x_angle)
            x_ave = sum(angle_list) / len(angle_list)
            goal.draw_rect(img, color = (0, 0, 255))
            uart.write(str(x_ave)+"\n")
            print(x_ave)
        else:
            uart.write(str(-1)+"\n")





