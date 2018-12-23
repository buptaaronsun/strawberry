// pages/douban/detail/detail.js
var wxCharts = require('../../../utils/wxcharts.js');
var app = getApp();
var daylineChart = null;
var tempChart = null;
var humidityChart= null;
var pressChart = null;
var heightChart = null;
var temp = [];
var humidity=[];
var press=[];
var height=[];
Page({

  /**
   * 页面的初始数据
   */
  data: {
    test: '',
    URL: 'http://creaters624.cn/week.php',
  },
  /**
   * 生命周期函数--监听页面加载
   */
  Erase:function(Array)
  {
    for (var i = 1; i <= 31; i++)
      Array.shift();
  },
  UpData: function (Array, Num) {
    Array.push(Num);
  },

  getMothElectro: function () {
    //for(var i=1;i<=30;i++)
    {
      var windowWidth = 320;
      var that = this;
      var wxcountry = "China";
      wx.request({

        url: this.data.URL,
        //method:'POST',
        header: {
          "content-Type": "applicationg/x-www-form-urlencoded"
        },

        success: function (res) {
          that.setData({
            //hidden: false,
            //data_from_mongodb: wxcountry + " : " + res.data.helloWorld,
            test: res.data,

          });
        that.Erase(temp);
        that.Erase(humidity);
        that.Erase(press);
        that.Erase(height);
        for(var i=30;i>=0;i--)
        {
          that.UpData(temp, parseInt(res.data[i].temp));
          that.UpData(humidity,parseInt(res.data[i].hum));
          that.UpData(press, parseInt(res.data[i].press)/1000);
          that.UpData(height, parseInt(res.data[i].height));
        }
          console.log(res.data);
          console.log(temp);
          console.log(humidity);
          console.log(press);
          console.log(height);
        },
        fail: function (res) {
          console.log("failed");
        },
        complete: function (res) {

        },
      })
      try {
        var res = wx.getSystemInfoSync();
        windowWidth = res.windowWidth;
      } catch (e) {
        console.error('getSystemInfoSync failed!');
      }
      tempChart = new wxCharts({ 
        canvasId: 'Tempc',
        type: 'line',
        categories: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'], //categories X轴
        animation: true,
        // background: '#f5f5f5',
        series: [{
          name: '温度',
          //data: yuesimulationData.data,

          data: temp,

          //data:data+'2',
          format: function (val, name) {
            return val.toFixed(2) + '°C';
          }
        }, ],
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
        width: windowWidth - 20,
        height: 200,
        dataLabel: false,
        dataPointShape: true,
        extra: {
          lineStyle: 'curve'
        }
      });
      humidityChart = new wxCharts({
        canvasId: 'Humidityc',
        type: 'line',
        categories: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'], //categories X轴
        animation: true,
        // background: '#f5f5f5',
        series:  [{
          name: '湿度',
          data: humidity,
          format: function (val, name) {
            return val.toFixed(2) + '%';
          }
        }, ] ,
        xAxis: {
          disableGrid: true
        },
        yAxis: {
          title: '湿度（%）',
          format: function (val) {
            return val.toFixed(2);
          },
          max: 20,
          min: 0
        },
        width: windowWidth - 20,
        height: 200,
        dataLabel: false,
        dataPointShape: true,
        extra: {
          lineStyle: 'curve'
        }
      });
      pressChart = new wxCharts({
        canvasId: 'Pressc',
        type: 'line',
        categories: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'], //categories X轴
        animation: true,
        // background: '#f5f5f5',
        series: [
          {
            name: '气压',
            data: press,
            format: function (val, name) {
              return val.toFixed(2) + 'kPa';
            }
          },
        ],
        xAxis: {
          disableGrid: true
        },
        yAxis: {
          title: '气压（kPa）',
          format: function (val) {
            return val.toFixed(2);
          },
          max: 20,
          min: 0
        },
        width: windowWidth - 20,
        height: 200,
        dataLabel: false,
        dataPointShape: true,
        extra: {
          lineStyle: 'curve'
        }
      });
      heightChart = new wxCharts({ 
        canvasId: 'Heightc',
        type: 'line',
        categories: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'], //categories X轴
        animation: true,
        // background: '#f5f5f5',
        series: [
        {
          name: '高度',
          data: height,
          format: function (val, name) {
            return val.toFixed(2) + 'm';
          }
        }],
        xAxis: {
          disableGrid: true
        },
        yAxis: {
          title: '高度(m)',
          format: function (val) {
            return val.toFixed(2);
          },
          max: 20,
          min: 0
        },
        width: windowWidth - 20,
        height: 200,
        dataLabel: false,
        dataPointShape: true,
        extra: {
          lineStyle: 'curve'
        }
      });
    }
  },
  TempTouchHandler: function (e) { //当月用电触摸显示
    console.log(tempChart.getCurrentDataIndex(e));
    tempChart.showToolTip(e, { //showToolTip图表中展示数据详细内容
      background: '#7cb5ec',
      format: function (item, category) {
        return category + '日 ' + item.name + ':' + item.data
      }
    });
  },
  HumidityTouchHandler: function (e) { //当月用电触摸显示
    console.log(humidityChart.getCurrentDataIndex(e));
    humidityChart.showToolTip(e, { //showToolTip图表中展示数据详细内容
      background: '#7cb5ec',
      format: function (item, category) {
        return category + '日 ' + item.name + ':' + item.data
      }
    });
  },
  PressTouchHandler: function (e) { //当月用电触摸显示
    console.log(pressChart.getCurrentDataIndex(e));
    pressChart.showToolTip(e, { //showToolTip图表中展示数据详细内容
      background: '#7cb5ec',
      format: function (item, category) {
        return category + '日 ' + item.name + ':' + item.data
      }
    });
  },
  HeightTouchHandler: function (e) { //当月用电触摸显示
    console.log(heightChart.getCurrentDataIndex(e));
    heightChart.showToolTip(e, { //showToolTip图表中展示数据详细内容
      background: '#7cb5ec',
      format: function (item, category) {
        return category + '日 ' + item.name + ':' + item.data
      }
    });
  },
  onLoad: function (options) {
    this.getMothElectro();
    var that = this;
    var wxcountry = "China";
    wx.request({

      url: this.data.URL,
      //method:'POST',
      header: {
        "content-Type": "applicationg/x-www-form-urlencoded"
      },

      success: function (res) {
        that.setData({
          //hidden: false,
          //data_from_mongodb: wxcountry + " : " + res.data.helloWorld,
          test: res.data,

        });
        //var testnum = parseInt(that.data.test);
        //that.UpData(data2, testnum);
        console.log(res.data.temp);
      },
      fail: function (res) {
        console.log("failed");
      },
      complete: function (res) {

      },
    })
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
   this.getMothElectro();
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