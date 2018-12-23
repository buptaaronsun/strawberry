// pages/douban/in_theathers/in_theathers.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    timage: 'hiddenimages',
    test: '',
    URL: 'http://192.168.43.226:8081',
    pic:'http://imga2.5054399.com/upload_pic/2015/9/1/4399_17562395347.gif'
  },
  switchHidden: function () {
    if (this.data.timage == 'images') {
      this.setData({
        timage: 'hiddenimages'
      })
    }
    else {
      this.setData({
        timage: 'images'
      })
    }
  },
  Switchpage: function () {
    wx.switchTab({
      url: '../coming_soon/coming_soon',
    })
  },
  Switchpage2: function () {
    wx.navigateTo({
      url: '../detail/detail',
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
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
        console.log(res);
      },
      fail:function(res)
      {
        console.log("failed");
      },
      complete: function (res) {
         console.log("failed"); 
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