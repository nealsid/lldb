//===-- PromptInterpolation.cpp ------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "lldb/Interpreter/PromptInterpolation.h"

#include "lldb/Core/Debugger.h"
#include "lldb/Target/Process.h"
#include "lldb/Target/RegisterContext.h"
#include "lldb/Target/StackFrame.h"
#include "lldb/Target/StopInfo.h"
#include "lldb/Target/Target.h"
#include "lldb/Target/Thread.h"

#include <sstream>

using namespace lldb;
using namespace lldb_private;

const std::string kFallbackPrompt = "(lldb) ";

namespace lldb_private {

std::string PromptInterpolation::InterpolatePrompt(
    const std::string& prompt_format) {
  std::stringstream interpolated_prompt;
  
  TargetSP target = m_debugger.GetSelectedTarget();

  if (!target) {
    interpolated_prompt << kFallbackPrompt;
    return interpolated_prompt.str();
  }

  StackFrameSP frame_sp;
  const SymbolContext *sc = nullptr;
  if (target->GetProcessSP()) {
    ThreadList& thread_list = target->GetProcessSP()->GetThreadList();
    frame_sp = thread_list.GetSelectedThread()->GetSelectedFrame();
    sc = &frame_sp->GetSymbolContext(eSymbolContextEverything);
  }
  ExecutionContext exe_ctx(frame_sp);

  FormatEntity::Entry root;
  Status parse_status = FormatEntity::Parse(prompt_format, root);
  bool formatted = false;
  StreamString strm;
  if (parse_status.Success()) {
    formatted = FormatEntity::Format(root, strm, sc, &exe_ctx,
                                     nullptr, nullptr, false, false);
    if (formatted) {
      interpolated_prompt << strm.GetData();
      return interpolated_prompt.str();
    } 
  }
  interpolated_prompt << kFallbackPrompt;
  return interpolated_prompt.str();
}

} // namespace lldb_private
