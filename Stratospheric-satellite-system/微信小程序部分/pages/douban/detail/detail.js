// pages/douban/detail/detail.js
var wxCharts = require('../../../utils/wxcharts.js');
var app = getApp();
var daylineChart = null;
var yuelineChart = null;
var data2 = [1, 6, 9, 1, 0, 2, 9, 2, 8, 6, 0, 9, 8, 0, 3, 7, 3, 9, 3, 8, 9, 5, 4, 1, 5, 8, 2, 4, 9, 8, 7];
Page({
  
  /**
   * 页面的初始数据
   */
  data: {
      
  },
  
  /**
   * 生命周期函数--监听页面加载
   */
  UpData:function(Array,Num) {
    Array.shift();
    Array.push(Num);
  },
  getMothElectro: function () {
    //for(var i=1;i<=30;i++)
    {
      var windowWidth = 320;
      var timestamp =
        Date.parse(new Date());
      timestamp = timestamp / 1000;
      //获取当前时间
      var n = timestamp * 1000;
      var date = new Date(n);
      var s = date.getSeconds();
      this.UpData(data2, s/2);
      try {
        var res = wx.getSystemInfoSync();
        windowWidth = res.windowWidth;
      } catch (e) {
        console.error('getSystemInfoSync failed!');
      }
      yuelineChart = new wxCharts({ //当月用电折线图配置
        canvasId: 'yueEle',
        type: 'line',
        categories: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'], //categories X轴
        animation: true,
        // background: '#f5f5f5',
        series: [{
          name: '温度',
          //data: yuesimulationData.data,
          
          data:data2,
          
          //data:data+'2',
          format: function (val, name) {
            return val.toFixed(2) + '°C';
          }
        },  /*{
          name: '电池供电量',
          data: [0, 6, 2, 2, 7, 6, 2, 5, 8, 1, 8, 4, 0, 9, 7, 2, 5, 2, 8, 2, 5, 2, 9, 4, 4, 9, 8, 5, 5, 5, 6],
          format: function (val, name) {
            return val.toFixed(2) + 'kWh';
          }
        },
        {
          name: '光伏供电量',
          data: [6, 4, 9, 7, 1, 4, 5, 1, 1, 8, 8, 6, 6, 2, 2, 2, 0, 5, 5, 8, 8, 8, 8, 9, 0, 4, 6, 9, 2, 1, 6],
          format: function (val, name) {
            return val.toFixed(2) + 'kWh';
          }
        },
        {
          name: '市电供电量',
          data: [0, 4, 3, 3, 1, 7, 7, 7, 9, 9, 3, 3, 0, 0, 5, 6, 0, 4, 1, 2, 0, 1, 3, 9, 2, 5, 1, 8, 3, 4, 2],
          format: function (val, name) {
            return val.toFixed(2) + 'kWh';
          }
        }*/],
        xAxis: {
          disableGrid: true
        },
        yAxis: {
          title: '温度（°C)',
          format: function (val) {
            return val.toFixed(2);
          },
          max: 20,
          min: 0
        },
        width: windowWidth-20,
        height: 200,
        dataLabel: false,
        dataPointShape: true,
        extra: {
          lineStyle: 'curve'
        }
      });
      
    }
  },
  onLoad: function (options) {
    this.getMothElectro();
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
      
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})