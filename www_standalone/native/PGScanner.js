var PGScanner = {
    scan: function(args, success, fail) {
        console.log("PGScanner.scan");
        
        //PhoneGap.exec(<<successCallback>>,<<failureCallback>>,<<Plugin Name>>,<<Action Name>>,<<Arguments Array>>);
        //Plugin Name is set in Phonegap.plist
        return PhoneGap.exec(success, fail, "PluginClass", "scan", args);
    }
};
//function foo()
//{
//    navigator.notification.alert("foo");
//}