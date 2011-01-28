/* cookie handling */

function createCookie(name, value, days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function eraseCookie(name) {
	createCookie(name,"",-1);
}

/* mobile/normal version switcher */

var viewportWidth = $(window).width();

function initViewportWidth() {
  var deviceWidth = $(window).width();
  
  if(viewportWidth == deviceWidth)
    viewportWidth = null;
}

function set2mobile() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=device-width");
    
  $("#navigation").hide();
  $("#stylesheet_mobile").attr("rel", "stylesheet");
  $("#navigation").show();
  $("#switch2mobile").hide();
  $("#switch2normal").show();
}

function set2normal() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=" + viewportWidth);
    
  $("#stylesheet_mobile").removeAttr("rel");
  $("#switch2normal").hide();
  $("#switch2mobile").show();
}

function switch2mobile() {
  createCookie("mobile", "true", 1);
  window.location.reload();
}

function switch2normal() {
  createCookie("mobile", "false", 1);
  window.location.reload();
}

/* initialization on document load */
var mobile = readCookie("mobile");

if(mobile == null)
  mobile = screen.width <= 480;
else
  mobile = mobile == "true";

function initMobileSwitcher() {
  $("#stylesheet_mobile").removeAttr("media");
  $("#footer").prepend(
    '<a id="switch2mobile" href="#body" class="version_switcher" style="display:none">switch to mobile version</a>' +
    '<a id="switch2normal" href="#body" class="version_switcher" style="display:none">switch to normal version</a>'
  );
  $("#switch2mobile").click(switch2mobile);
  $("#switch2normal").click(switch2normal);
  
  if(mobile)
    set2mobile();
  else
    set2normal();
}

function showNavItem() {
 $(this).children(".nav-inactive").slideDown();
}

function hideNavItem(navItem) {
 $(this).children(".nav-inactive").slideUp();
}

$(document).ready(function() {
  $("#javascript-warning").remove();
  $(".nav-item").hover(showNavItem, hideNavItem);
  initMobileSwitcher();
});

