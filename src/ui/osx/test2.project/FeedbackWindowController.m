//
//  FeedbackWindowController.m
//  kopsik_ui_osx
//
//  Created by Tanel Lebedev on 29/01/2014.
//  Copyright (c) 2014 TogglDesktop developers. All rights reserved.
//

#import "FeedbackWindowController.h"
#import "kopsik_api.h"
#import "Context.h"
#import "ErrorHandler.h"

@interface FeedbackWindowController ()

@end

@implementation FeedbackWindowController

- (IBAction)uploadImageClick:(id)sender {
  NSOpenPanel* panel = [NSOpenPanel openPanel];
  [panel beginWithCompletionHandler:^(NSInteger result){
    if (result == NSFileHandlingPanelOKButton) {
      NSURL *url = [[panel URLs] objectAtIndex:0];
      NSLog(@"Selected document: %@", url);
      [self.selectedImageTextField setStringValue:[url path]];
      [self.selectedImageTextField setHidden:NO];
    }
  }];
}

- (IBAction)sendClick:(id)sender {
  if (self.topicComboBox.stringValue == nil
      || [self.topicComboBox.stringValue isEqualToString:@""]) {
    [self.topicComboBox becomeFirstResponder];
    return;
  }
  if (self.contentTextView.string == nil
      || [self.contentTextView.string isEqualToString:@""]) {
    [self.contentTextView becomeFirstResponder];
    return;
  }
  
  char errmsg[KOPSIK_ERR_LEN];
  kopsik_api_result res =
    kopsik_feedback_send(ctx,
                        errmsg,
                        KOPSIK_ERR_LEN,
                        [self.topicComboBox.stringValue UTF8String],
                        [self.contentTextView.string UTF8String],
                        [self.selectedImageTextField.stringValue UTF8String]);
  if (res != KOPSIK_API_SUCCESS) {
    handle_error(errmsg);
    return;
  }

  [self.window close];
  [self.selectedImageTextField setStringValue:@""];
  [self.selectedImageTextField setHidden:YES];
  [self.contentTextView setString:@""];
  [self.topicComboBox setStringValue:@""];
}

@end
